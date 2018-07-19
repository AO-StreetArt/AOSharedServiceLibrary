/*
MIT License Block

Copyright (c) 2016 Alex Barry

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>
#include <vector>
#include <utility>
#include <iterator>
#include <sstream>
#include <fstream>

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

#include "aossl/core/include/kv_store.h"
#include "aossl/core/include/kv_store_interface.h"
#include "aossl/profile/include/app_profile.h"
#include "aossl/profile/include/safe_app_profile.h"
#include "aossl/vault/include/vault_interface.h"
#include "aossl/vault/include/factory_vault.h"

#ifndef AOSSL_PROFILE_INCLUDE_TIERED_APP_PROFILE_H_
#define AOSSL_PROFILE_INCLUDE_TIERED_APP_PROFILE_H_

namespace AOSSL {

//! Biased Application Session which implements tiered configuration.

//! Tiered Configuration prioritizes config values in the following order:
//! 1. Command Line Arguments
//! 2. Environment variables
//! 3. Consul Key-Value store variables
//! 4. Properties File values
//! 5. Default values
class TieredApplicationProfile: public SafeApplicationProfile{
  std::string props_file_name;
  std::string cluster_name;
  std::string configuration_key_start;
  std::vector<std::string> secure_opt_keys;
  std::vector<std::string> config_record;
  inline bool exists_test(const std::string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
  }

  // Load a value from a KV Store
  inline void load_config_value(KeyValueStoreInterface *kv, std::string& key) {
    if (kv) {
      if (kv->opt_exist(key)) {
        AOSSL::StringBuffer buf;
        kv->get_opt(key, buf);
        KeyValueStore::set_opt(key, buf.val);
        config_record.push_back(std::string("Retrieved Record: ") + buf.val \
            + std::string(" for key: ") + key);
      }
    }
  }

  // Retrieve a secret from Vault
  inline void get_vault_secret(KeyValueStoreInterface *kv, std::string& key, StringBuffer& return_buf) {
    StringBuffer buf;
    kv->get_opt(key, buf);
    if (buf.success && !(buf.val.empty())) {
      // Parse out the data and compare it
      rapidjson::Document d;
      d.Parse<rapidjson::kParseStopWhenDoneFlag>(buf.val.c_str());
      if (d.HasParseError()) {
        return_buf.success = false;
        return_buf.err_msg.assign(GetParseError_En(d.GetParseError()));
      } else {
        if (d.IsObject()) {
          const rapidjson::Value& token_val = d["data"]["data"][key.c_str()];
          return_buf.val.assign(token_val.GetString());
          config_record.push_back(std::string("Retrieved Record for key: ") \
              + key + std::string(" from Vault"));
        }
      }
    } else {
      return_buf.success = false;
      return_buf.err_msg.assign(buf.err_msg);
    }
  }

  // Load a value from Vault into configuration
  inline void load_vault_secret(KeyValueStoreInterface *kv, std::string& key) {
    if (kv) {
      StringBuffer buf;
      std::string vault_key = key;
      // Convert to all caps
      std::transform(vault_key.begin(), vault_key.end(), vault_key.begin(), toupper);
      // Convert '.' to '_'
      std::replace(vault_key.begin(), vault_key.end(), '.', '_');
      get_vault_secret(kv, vault_key, buf);
      if (buf.success) {
        KeyValueStore::set_opt(key, buf.val);
      } else {
        config_record.push_back(std::string("Failed to retrieve Vault Secret: ") \
            + buf.err_msg);
      }
    }
  }

  // Load a value from Consul
  inline void load_consul_value(ConsulInterface *kv, std::string& key) {
    if (kv) {
      if (key.empty()) return;
      // Copy the key before modifying in-place
      std::string env_key = key;
      // Convert to all caps
      std::transform(env_key.begin(), env_key.end(), env_key.begin(), toupper);
      // Convert '.' to '_'
      std::replace(env_key.begin(), env_key.end(), '.', '_');
      std::string query_key;
      // Prefix the key with the application & profile name
      // to ensure we get unique values for different apps
      if (!(ApplicationProfile::get_app_name().empty() && \
          ApplicationProfile::get_profile_name().empty())) {
        query_key = ApplicationProfile::get_app_name() + std::string("/") + \
            ApplicationProfile::get_profile_name() + std::string("/");
      }
      query_key = query_key + env_key;
      if (kv->opt_exist(query_key)) {
        AOSSL::StringBuffer buf;
        kv->get_opt(query_key, buf);
        if (!(buf.val.empty()) && buf.success) {
          // Parse the response
          rapidjson::Document d;
          d.Parse<rapidjson::kParseStopWhenDoneFlag>(buf.val.c_str());
          if (d.HasParseError()) {
            throw std::invalid_argument(GetParseError_En(d.GetParseError()));
          }
          // Pull the value field
          StringBuffer parsed_buffer;
          std::string value_string;
          if (d.IsArray()) {
            for (auto& itr : d.GetArray()) {
              rapidjson::Value::ConstMemberIterator val_iter = \
                  itr.FindMember("Value");
              if (val_iter != itr.MemberEnd()) {
                if (!(val_iter->value.IsNull())) {
                  parsed_buffer.val.assign(val_iter->value.GetString());
                }
              }
            }
          }
          // decode the base64 value
          StringBuffer decoded_buffer;
          ApplicationProfile::get_consul()->base64_decode_by_reference(\
            parsed_buffer.val, decoded_buffer);
          if (KeyValueStore::opt_exist(key)) {
            KeyValueStore::set_opt(key, decoded_buffer.val);
            config_record.push_back(std::string("Retrieved Record: ") + decoded_buffer.val \
                + std::string(" for key: ") + key + std::string(" from Consul"));
          }
        }
      }
    }
  }

  // Load a value from an environment variable
  inline void load_environment_variable(std::string& key) {
    // Copy the key before modifying in-place
    std::string env_key = key;
    // Convert to all caps
    std::transform(env_key.begin(), env_key.end(), env_key.begin(), toupper);
    // Convert '.' to '_'
    std::replace(env_key.begin(), env_key.end(), '.', '_');
    // Get the environment variable
    const char *env_value = std::getenv(env_key.c_str());
    if (env_value && KeyValueStore::opt_exist(key)) {
      std::string env_str(env_value);
      KeyValueStore::set_opt(key, env_str);
      config_record.push_back(std::string("Retrieved Record: ") + env_str \
          + std::string(" for key: ") + key + std::string(" from Environment Variable"));
    }
  }

  inline void load_vault_info(KeyValueStoreInterface *kv, \
      bool auth_details_included, std::string& un, std::string& pw) {
    if (kv) {
      // Check the commandline arguments for Vault Information
      std::string vault_addr_key = configuration_key_start + "vault";
      std::string vault_cert_key = configuration_key_start + "vault.cert";
      std::string vault_atype_key = configuration_key_start + "vault.authtype";
      std::string vault_un_key = configuration_key_start + "vault.un";
      std::string vault_pw_key = configuration_key_start + "vault.pw";
      std::string secrets_path("/v1/secret/data/");
      int auth_type = BASIC_AUTH_TYPE;
      StringBuffer vault_addr_buf;
      StringBuffer vault_cert_buf;
      StringBuffer vault_authtype_buf;
      StringBuffer vault_un_buf;
      StringBuffer vault_pw_buf;
      // Check for authentication info
      if (auth_details_included || (kv->opt_exist(vault_un_key) && kv->opt_exist(vault_pw_key))) {
        if (auth_details_included) {
          vault_un_buf.val.assign(un);
          vault_pw_buf.val.assign(pw);
        } else {
          kv->get_opt(vault_un_key, vault_un_buf);
          kv->get_opt(vault_pw_key, vault_pw_buf);
        }
        // Check for the rest of the config info for Vault
        if (kv->opt_exist(vault_addr_key) && kv->opt_exist(vault_cert_key) \
            && kv->opt_exist(vault_atype_key)) {
          kv->get_opt(vault_addr_key, vault_addr_buf);
          kv->get_opt(vault_cert_key, vault_cert_buf);
          kv->get_opt(vault_atype_key, vault_authtype_buf);
          if (vault_authtype_buf.val == "APPROLE") {
            auth_type = APPROLE_AUTH_TYPE;
          }
          config_record.push_back(std::string("Setting Vault Information: ") \
              + vault_addr_buf.val);
          ApplicationProfile::set_vault_address(vault_addr_buf.val, secrets_path, \
              5, vault_cert_buf.val, auth_type, vault_un_buf.val, vault_pw_buf.val);
        } else if (kv->opt_exist(vault_addr_key) && kv->opt_exist(vault_atype_key)) {
          kv->get_opt(vault_addr_key, vault_addr_buf);
          kv->get_opt(vault_atype_key, vault_authtype_buf);
          kv->get_opt(vault_un_key, vault_un_buf);
          kv->get_opt(vault_pw_key, vault_pw_buf);
          if (vault_authtype_buf.val == "APPROLE") {
            auth_type = APPROLE_AUTH_TYPE;
          }
          config_record.push_back(std::string("Setting Vault Information: ") \
              + vault_addr_buf.val);
          ApplicationProfile::set_vault_address(vault_addr_buf.val, secrets_path, \
              5, auth_type, vault_un_buf.val, vault_pw_buf.val);
        }
      }
    }
  }

  inline void load_consul_info(KeyValueStoreInterface *kv, \
      StringBuffer& vconsul_cert_buf, StringBuffer& vconsul_token_buf) {
    if (kv) {
      // Check the commandline arguments for Consul Information
      std::string consul_add_key = configuration_key_start + "consul";
      std::string consul_cert_key = configuration_key_start + "consul.cert";
      std::string consul_token_key = configuration_key_start + "consul.token";
      StringBuffer consul_addr_buf;
      StringBuffer consul_cert_buf;
      StringBuffer consul_token_buf;
      // We have a pre-existing cert and token, and have been given an address
      if (kv->opt_exist(consul_add_key) && vconsul_cert_buf.success && \
          vconsul_token_buf.success) {
        kv->get_opt(consul_add_key, consul_addr_buf);
        config_record.push_back(std::string("Setting Consul Information: ") \
            + consul_addr_buf.val);
        ApplicationProfile::set_consul_address(consul_addr_buf.val, 5, \
            vconsul_cert_buf.val, vconsul_token_buf.val);
      // We have been given an address, cert, and a pre-existing token
      } else if (kv->opt_exist(consul_add_key) && kv->opt_exist(consul_cert_key) && \
          vconsul_token_buf.success) {
        kv->get_opt(consul_add_key, consul_addr_buf);
        kv->get_opt(consul_cert_key, consul_cert_buf);
        config_record.push_back(std::string("Setting Consul Information: ") \
            + consul_addr_buf.val);
        ApplicationProfile::set_consul_address(consul_addr_buf.val, 5, \
            consul_cert_buf.val, vconsul_token_buf.val);
      // We have been given an address, cert, and a token
      } else if (kv->opt_exist(consul_add_key) && \
          kv->opt_exist(consul_cert_key) && \
          kv->opt_exist(consul_token_key)) {
        config_record.push_back(std::string("Setting Consul Information: ") \
            + consul_addr_buf.val);
        kv->get_opt(consul_add_key, consul_addr_buf);
        kv->get_opt(consul_cert_key, consul_cert_buf);
        kv->get_opt(consul_token_key, consul_token_buf);
        ApplicationProfile::set_consul_address(consul_addr_buf.val, 5, \
            consul_cert_buf.val, consul_token_buf.val);
      // We have been given an address and a cert
      } else if (kv->opt_exist(consul_add_key) && \
          (kv->opt_exist(consul_cert_key) || vconsul_cert_buf.success)) {
        kv->get_opt(consul_add_key, consul_addr_buf);
        if (vconsul_cert_buf.success) {
          consul_cert_buf.val = vconsul_cert_buf.val;
          consul_cert_buf.success = true;
        } else {
          kv->get_opt(consul_cert_key, consul_cert_buf);
        }
        config_record.push_back(std::string("Secure Consul Connection: ") \
            + consul_addr_buf.val);
        ApplicationProfile::set_consul_address(consul_addr_buf.val, 5, \
            consul_cert_buf.val);
      // We have been given an address and a token
      } else if (kv->opt_exist(consul_add_key) && \
          (kv->opt_exist(consul_token_key) || vconsul_token_buf.success)) {
        kv->get_opt(consul_add_key, consul_addr_buf);
        if (vconsul_token_buf.success) {
          consul_token_buf.val = vconsul_token_buf.val;
          consul_token_buf.success = true;
        } else {
          kv->get_opt(consul_token_key, consul_token_buf);
        }
        config_record.push_back(std::string("Authenticated Consul Connection: ") \
            + consul_addr_buf.val);
        ApplicationProfile::set_consul_address(consul_addr_buf.val);
        config_record.push_back(std::string("Adding ACL Token: ") \
            + consul_addr_buf.val);
        ApplicationProfile::get_consul()->add_acl_token(consul_token_buf.val);
      // We've been given an address
      } else if (kv->opt_exist(consul_add_key)) {
        kv->get_opt(consul_add_key, consul_addr_buf);
        config_record.push_back(std::string("Setting Consul Information: ") \
            + consul_addr_buf.val);
        ApplicationProfile::set_consul_address(consul_addr_buf.val);
      }
    }
  }

  // Initialize the Profile
  inline void init() {
    // See if we have a profile name provided
    if (ApplicationProfile::get_cli()) {
      if (ApplicationProfile::get_cli()->opt_exist(std::string("profile"))) {
        StringBuffer profile_name_buf;
        ApplicationProfile::get_cli()->get_opt(std::string("profile"), profile_name_buf);
        ApplicationProfile::set_profile_name(profile_name_buf.val);
      }
    }

    // What should the start of our config strings be?
    std::string config_key_start;
    if (!(ApplicationProfile::get_app_name().empty())) {
      config_key_start += ApplicationProfile::get_app_name();
      config_key_start += std::string(".");
    }
    if (!(ApplicationProfile::get_profile_name().empty())) {
      config_key_start += ApplicationProfile::get_profile_name();
      config_key_start += std::string(".");
    }
    config_record.push_back(config_key_start);
    configuration_key_start.assign(config_key_start);
    std::string env_config_key_start(config_key_start);
    std::transform(env_config_key_start.begin(), env_config_key_start.end(), \
        env_config_key_start.begin(), toupper);
    std::replace(env_config_key_start.begin(), env_config_key_start.end(), '.', '_');
    // Start by looking for properties files, as values for other configs can
    // be stored in the props files.

    // Check Env Variables for a Properties File
    std::string props_env_key = env_config_key_start + std::string("PROPS_FILE");
    const char *env_props_value = std::getenv(props_env_key.c_str());
    if (env_props_value) {
      std::string props_file_str(env_props_value);
      ApplicationProfile::set_property_file(props_file_str);
      config_record.push_back(std::string("Setting Properties File: ") \
          + props_file_str);
    }

    // Check the commandline arguments for a properties file
    std::string props_key = config_key_start + std::string("props");
    if (ApplicationProfile::get_cli()) {
      if (ApplicationProfile::get_cli()->opt_exist(props_key)) {
        StringBuffer props_buf;
        ApplicationProfile::get_cli()->get_opt(props_key, props_buf);
        ApplicationProfile::set_property_file(props_buf.val);
        config_record.push_back(std::string("Setting Properties File: ") \
            + props_buf.val);
      }
    }

    // Check for default property files
    props_file_name = "/etc/ivan/app.properties";
    if ((!(ApplicationProfile::get_props())) && exists_test(props_file_name)) {
      ApplicationProfile::set_property_file(props_file_name);
      config_record.push_back(std::string("Setting Properties File: ") \
          + props_file_name);
    }
    props_file_name = "app.properties";
    if ((!(ApplicationProfile::get_props())) && exists_test(props_file_name)) {
      ApplicationProfile::set_property_file(props_file_name);
      config_record.push_back(std::string("Setting Properties File: ") \
          + props_file_name);
    }

    // Next, we want to check for Vault information, as further
    // security information needed for configuration may be held there.

    std::string vault_un_file = "vault_un.txt";
    std::string vault_pw_file = "vault_pw.txt";
    std::string vault_un_value;
    std::string vault_pw_value;
    bool vault_auth_from_file = false;

    // See if we have vault username/password files
    if (exists_test(vault_un_file) && exists_test(vault_pw_file)) {
      std::ifstream unfs(vault_un_file);
      vault_un_value.assign((std::istreambuf_iterator<char>(unfs)), \
          (std::istreambuf_iterator<char>()));
      std::ifstream pwfs(vault_pw_file);
      vault_pw_value.assign((std::istreambuf_iterator<char>(pwfs)), \
          (std::istreambuf_iterator<char>()));
      vault_auth_from_file = true;
      config_record.push_back(std::string("Using Vault authentication info from files"));
    }

    // Check environment variables for vault information
    std::string env_vault_addr_key = env_config_key_start + std::string("VAULT_ADDRESS");
    std::string env_vault_cert_key = env_config_key_start + std::string("VAULT_SSL_CERT");
    std::string env_vault_authtype_key = env_config_key_start + std::string("VAULT_AUTH_TYPE");
    std::string env_vault_authun_key = env_config_key_start + std::string("VAULT_AUTH_UN");
    std::string env_vault_authpw_key = env_config_key_start + std::string("VAULT_AUTH_PW");
    const char *env_vault_addr = std::getenv(env_vault_addr_key.c_str());
    const char *env_vault_cert = std::getenv(env_vault_cert_key.c_str());
    const char *env_vault_authtype = std::getenv(env_vault_authtype_key.c_str());
    const char *env_vault_authun = std::getenv(env_vault_authun_key.c_str());
    const char *env_vault_authpw = std::getenv(env_vault_authpw_key.c_str());
    std::string un;
    std::string pw;
    if (env_vault_addr && env_vault_cert && env_vault_authtype) {
      if (vault_auth_from_file || (env_vault_authun && env_vault_authpw)) {
        if (vault_auth_from_file) {
          un.assign(vault_un_value);
          pw.assign(vault_pw_value);
        } else {
          un.assign(env_vault_authun);
          pw.assign(env_vault_authpw);
        }
        std::string vaddr(env_vault_addr);
        std::string cert(env_vault_cert);
        std::string secrets_path("/v1/secret/data/");
        std::string authtype_string(env_vault_authtype);
        int auth_type = BASIC_AUTH_TYPE;
        if (authtype_string == "APPROLE") {
          auth_type = APPROLE_AUTH_TYPE;
        }
        config_record.push_back(std::string("Setting Vault Information from Environment Variables: ") \
            + vaddr);
        ApplicationProfile::set_vault_address(vaddr, secrets_path, 5, cert, auth_type, un, pw);
      }
    } else if (env_vault_addr && env_vault_authtype && env_vault_authun && env_vault_authpw) {
      if (vault_auth_from_file) {
        un.assign(vault_un_value);
        pw.assign(vault_pw_value);
      } else {
        un.assign(env_vault_authun);
        pw.assign(env_vault_authpw);
      }
      std::string vaddr(env_vault_addr);
      std::string secrets_path("/v1/secret/data/");
      std::string authtype_string(env_vault_authtype);
      int auth_type = BASIC_AUTH_TYPE;
      if (authtype_string == "APPROLE") {
        auth_type = APPROLE_AUTH_TYPE;
      }
      config_record.push_back(std::string("Setting Vault Information from Environment Variables: ") \
          + vaddr);
      ApplicationProfile::set_vault_address(vaddr, secrets_path, 5, auth_type, un, pw);
    }
    // Try to load vault information from the CLI arguments
    load_vault_info(ApplicationProfile::get_cli(), vault_auth_from_file, vault_un_value, vault_pw_value);
    // Try to load vault information from the Properties File
    load_vault_info(ApplicationProfile::get_props(), vault_auth_from_file, vault_un_value, vault_pw_value);

    // Next, we'll check for any Consul information

    // Start by checking Vault for any SSL Certs or ACL Tokens.
    std::string consul_cert_vault_key = env_config_key_start \
        + std::string("CONSUL_SSL_CERT");
    std::string consul_token_vault_key = env_config_key_start \
        + std::string("CONSUL_ACL_TOKEN");
    StringBuffer consul_cert_buf;
    StringBuffer consul_token_buf;
    if (ApplicationProfile::get_vault()) {
      config_record.push_back(std::string("Checking Vault for Consul Security info"));
      get_vault_secret(ApplicationProfile::get_vault(), consul_cert_vault_key, consul_cert_buf);
      get_vault_secret(ApplicationProfile::get_vault(), consul_token_vault_key, consul_token_buf);
    }

    // See if we've been instructed to generate the Consul ACL token
    // from the Vault Consul Secrets Engine, rather than the KV Store
    bool generate_consul_token = false;
    StringBuffer gen_consul_token_buf;
    if (ApplicationProfile::get_cli()) {
      std::string gen_consul_token_key = config_key_start + "consul.token.role";
      if (ApplicationProfile::get_cli()->opt_exist(gen_consul_token_key)) {
        generate_consul_token = true;
        ApplicationProfile::get_cli()->get_opt(gen_consul_token_key, gen_consul_token_buf);
      }
    }

    std::string env_get_consul_token_key = env_config_key_start \
        + std::string("CONSUL_TOKEN_ROLE");
    const char *env_gen_consul_token = std::getenv(env_get_consul_token_key.c_str());
    if (env_gen_consul_token) {
      gen_consul_token_buf.val.assign(env_gen_consul_token);
      gen_consul_token_buf.success = true;
      generate_consul_token = true;
    }

    if (generate_consul_token && gen_consul_token_buf.success) {
      config_record.push_back("Generating Consul ACL Token with role: " + gen_consul_token_buf.val);
      ApplicationProfile::get_vault()->gen_consul_token(gen_consul_token_buf.val, \
          consul_token_buf);
      if (!(consul_token_buf.success)) {
        config_record.push_back(std::string("Failed to generate Consul ACL Token: ") \
            + consul_token_buf.err_msg);
      }
    }

    // Check the Properties file for the Consul args
    load_consul_info(ApplicationProfile::get_props(), consul_cert_buf, consul_token_buf);

    // Check Env Variables for Consul Information
    std::string env_consul_value_key = env_config_key_start \
        + std::string("CONSUL_ADDRESS");
    std::string env_consul_cert_key = env_config_key_start \
        + std::string("CONSUL_SSL_CERT");
    std::string env_consul_token_key = env_config_key_start \
        + std::string("CONSUL_ACL_TOKEN");
    const char *env_consul_value = std::getenv(env_consul_value_key.c_str());
    const char *env_consul_cert_value = std::getenv(env_consul_cert_key.c_str());
    const char *env_consul_token_value = std::getenv(env_consul_token_key.c_str());
    if (env_consul_value && env_consul_cert_value && env_consul_token_value) {
      std::string consul_addr_str(env_consul_value);
      std::string consul_cert_str(env_consul_cert_value);
      std::string consul_token_str(env_consul_token_value);
      config_record.push_back(std::string("Setting Consul Address: ") + consul_addr_str);
      ApplicationProfile::set_consul_address(consul_addr_str, 5, consul_cert_str, env_consul_value);
    } else if (env_consul_value && env_consul_cert_value) {
      std::string consul_addr_str(env_consul_value);
      std::string consul_cert_str(env_consul_cert_value);
      config_record.push_back(std::string("Setting Consul Address: ") + consul_addr_str);
      ApplicationProfile::set_consul_address(consul_addr_str, 5, consul_cert_str);
    } else if (env_consul_value) {
      // Mix environment variables with token/certs retrieved from Vault
      std::string consul_addr_str(env_consul_value);
      if (consul_cert_buf.success && consul_token_buf.success) {
        config_record.push_back(std::string("Setting Consul Address: ") + consul_addr_str);
        ApplicationProfile::set_consul_address(consul_addr_str, 5, consul_cert_buf.val, consul_token_buf.val);
      } else if (consul_cert_buf.success && env_consul_token_value) {
        std::string consul_token_str(env_consul_token_value);
        config_record.push_back(std::string("Setting Consul Address: ") + consul_addr_str);
        ApplicationProfile::set_consul_address(consul_addr_str, 5, consul_cert_buf.val, consul_token_str);
      } else if (env_consul_cert_value && consul_token_buf.success) {
        std::string consul_cert_str(env_consul_cert_value);
        config_record.push_back(std::string("Setting Consul Address: ") + consul_addr_str);
        ApplicationProfile::set_consul_address(consul_addr_str, 5, consul_cert_str, consul_token_buf.val);
      } else {
      config_record.push_back(std::string("Setting Consul Address: ") + consul_addr_str);
        ApplicationProfile::set_consul_address(consul_addr_str);
      }
    }

    // Check the commandline arguments for the Consul args
    load_consul_info(ApplicationProfile::get_cli(), consul_cert_buf, consul_token_buf);

    // Check for a provided Cluster Name
    std::string cluster_key = config_key_start + std::string("cluster");
    std::string env_cluster_key = env_config_key_start + std::string("CLUSTER");
    if (ApplicationProfile::get_props()) {
      if (ApplicationProfile::get_props()->opt_exist(cluster_key)) {
        StringBuffer cluster_name_buf;
        ApplicationProfile::get_props()->get_opt(cluster_key, cluster_name_buf);
        cluster_name.assign(cluster_name_buf.val);
        config_record.push_back(std::string("Setting Cluster Name: ") + cluster_name);
      }
    }
    if (ApplicationProfile::get_cli()) {
      if (ApplicationProfile::get_cli()->opt_exist(cluster_key)) {
        StringBuffer cluster_name_buf;
        ApplicationProfile::get_cli()->get_opt(cluster_key, cluster_name_buf);
        cluster_name.assign(cluster_name_buf.val);
        config_record.push_back(std::string("Setting Cluster Name: ") + cluster_name);
      }
    }
    const char *env_cluster_name = std::getenv(env_cluster_key.c_str());
    if (env_cluster_name) {
      cluster_name.assign(env_cluster_name);
      config_record.push_back(std::string("Setting Cluster Name: ") + cluster_name);
    }
  }

 public:
  TieredApplicationProfile(int argc, char* argv[]) : \
      SafeApplicationProfile(argc, argv) {init();}
  TieredApplicationProfile(int argc, char* argv[], std::string app_name, \
      std::string prof_name) : \
      SafeApplicationProfile(argc, argv, app_name, prof_name) {init();}
  explicit TieredApplicationProfile(const std::vector<std::string>& args) : \
      SafeApplicationProfile(args) {init();}
  TieredApplicationProfile(const std::vector<std::string>& args, \
      std::string app_name, std::string prof_name) : \
      SafeApplicationProfile(args, app_name, prof_name) {init();}
  TieredApplicationProfile(std::string app_name, std::string prof_name) : \
      SafeApplicationProfile(app_name, prof_name) {init();}

  virtual ~TieredApplicationProfile() {}

  //! Load the configuration from the various sources
  inline void load_config() {
    config_record.clear();
    config_record.push_back(std::string("Loading Configuration"));
    config_record.push_back(configuration_key_start);
    // Update the configuration for the properties file reader
    if (ApplicationProfile::get_props()) {
      ApplicationProfile::get_props()->load_config();
    }
    // Iterate over the default values and pull
    // values from available sources with the same key
    for (std::pair<std::string, std::string> element : \
        KeyValueStore::get_opts()) {
      config_record.push_back(std::string("Loading Records for Key: ") \
          + element.first);
      // Load Properties File values, if present
      load_config_value(ApplicationProfile::get_props(), element.first);
      // Load Consul Values, if present
      ConsulInterface *consul = ApplicationProfile::get_consul();
      load_consul_value(consul, element.first);
      // Load Environment variables
      load_environment_variable(element.first);
      // Load Commandline Values, if present
      load_config_value(ApplicationProfile::get_cli(), element.first);
    }
    // Get secure opts
    for (auto& secure_opt : secure_opt_keys) {
      // Load Vault Secret, if present
      load_vault_secret(ApplicationProfile::get_vault(), secure_opt);
    }
  }

  //! Add a secure configuration value to search for
  //! These are prioritized in the following order:
  //! 1. Environment variables
  //! 2. Properties File values
  //! 3. Vault Secret Values
  inline void add_secure_opt(std::string& key) {
    std::string final_key = configuration_key_start + key;
    std::string blank_val;
    secure_opt_keys.push_back(final_key);
    KeyValueStore::add_opt(final_key, blank_val);
  }

  //! Get the Cluster Name
  std::string get_cluster_name() {return cluster_name;}

  //! Set the Cluster Name
  void set_cluster_name(std::string& new_name) {cluster_name.assign(new_name);}

  //! Get the latest Configuration Record
  std::vector<std::string> get_config_record() {return config_record;}

  //! Get an option by key
  inline void get_opt(std::string key, StringBuffer& val) {
    std::string final_key = configuration_key_start + key;
    KeyValueStore::get_opt(final_key, val);
  }

  //! Add an option
  inline void add_opt(const std::string& key, const std::string& val) {
    std::string final_key = configuration_key_start + key;
    KeyValueStore::add_opt(final_key, val);
  }

  //! Set an option
  inline void set_opt(std::string& key, std::string& val) {
    std::string final_key = configuration_key_start + key;
    KeyValueStore::set_opt(final_key, val);
  }

  //! Does an option exist?
  inline bool opt_exist(std::string& key) {
    std::string final_key = configuration_key_start + key;
    return KeyValueStore::opt_exist(final_key);
  }
};

}  // namespace AOSSL

#endif  // AOSSL_PROFILE_INCLUDE_TIERED_APP_PROFILE_H_
