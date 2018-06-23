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

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

#include "aossl/core/include/kv_store.h"
#include "aossl/core/include/kv_store_interface.h"
#include "aossl/profile/include/app_profile.h"
#include "aossl/profile/include/safe_app_profile.h"
#include "aossl/vault/include/vault_admin.h"
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
  std::vector<std::string> secure_opt_keys;
  VaultComponentFactory vault_factory;
  KeyValueStoreInterface *vault = nullptr;
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
      }
    }
  }

  // Load a value from Vault
  inline void load_vault_secret(KeyValueStoreInterface *kv, std::string& key) {
    if (kv) {
      AOSSL::StringBuffer buf;
      kv->get_opt(key, buf);
      if (buf.success && !(buf.val.empty())) {
        // Parse out the data and compare it
        std::string data;
        rapidjson::Document d;
        d.Parse<rapidjson::kParseStopWhenDoneFlag>(buf.val.c_str());
        if (d.HasParseError()) {
          throw std::invalid_argument(GetParseError_En(d.GetParseError()));
        }
        if (d.IsObject()) {
          const rapidjson::Value& token_val = d["data"]["data"][key.c_str()];
          data.assign(token_val.GetString());
        }
        KeyValueStore::set_opt(key, data);
      } else {
        throw std::invalid_argument(buf.err_msg);
      }
    }
  }

  // Load a value from Consul
  inline void load_consul_value(ConsulInterface *kv, std::string& key) {
    if (kv) {
      if (key.empty()) return;
      // Copy the key before modifying in-place
      std::string env_key;
      env_key.assign(key);
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
          }
        }
      }
    }
  }

  // Load a value from an environment variable
  inline void load_environment_variable(std::string& key) {
    // Copy the key before modifying in-place
    std::string env_key;
    env_key.assign(key);
    // Convert to all caps
    std::transform(env_key.begin(), env_key.end(), env_key.begin(), toupper);
    // Convert '.' to '_'
    std::replace(env_key.begin(), env_key.end(), '.', '_');
    // Get the environment variable
    const char *env_value = std::getenv(env_key.c_str());
    if (env_value && KeyValueStore::opt_exist(key)) {
      std::string env_str(env_value);
      KeyValueStore::set_opt(key, env_str);
    }
  }

  inline void load_vault_info(KeyValueStoreInterface *kv) {
    if (kv) {
      // Check the commandline arguments for Vault Information
      std::string vault_addr_key = "vault";
      std::string vault_cert_key = "vault.cert";
      std::string vault_atype_key = "vault.authtype";
      std::string vault_un_key = "vault.un";
      std::string vault_pw_key = "vault.pw";
      std::string secrets_path("/v1/secret/data/");
      int auth_type = BASIC_AUTH_TYPE;
      StringBuffer vault_addr_buf;
      StringBuffer vault_cert_buf;
      StringBuffer vault_authtype_buf;
      StringBuffer vault_un_buf;
      StringBuffer vault_pw_buf;
      if (kv->opt_exist(vault_addr_key) && kv->opt_exist(vault_cert_key) \
          && kv->opt_exist(vault_atype_key) && kv->opt_exist(vault_un_key) \
          && kv->opt_exist(vault_pw_key)) {
        kv->get_opt(vault_addr_key, vault_addr_buf);
        kv->get_opt(vault_cert_key, vault_cert_buf);
        kv->get_opt(vault_atype_key, vault_authtype_buf);
        kv->get_opt(vault_un_key, vault_un_buf);
        kv->get_opt(vault_pw_key, vault_pw_buf);
        if (vault_authtype_buf.val == "APPROLE") {
          auth_type = APPROLE_AUTH_TYPE;
        }
        vault = vault_factory.get_vault_interface(vault_addr_buf.val, secrets_path, \
            5, vault_cert_buf.val, auth_type, vault_un_buf.val, vault_pw_buf.val);
      } else if (kv->opt_exist(vault_addr_key) && kv->opt_exist(vault_atype_key) \
          && kv->opt_exist(vault_un_key) && kv->opt_exist(vault_pw_key)) {
        kv->get_opt(vault_addr_key, vault_addr_buf);
        kv->get_opt(vault_atype_key, vault_authtype_buf);
        kv->get_opt(vault_un_key, vault_un_buf);
        kv->get_opt(vault_pw_key, vault_pw_buf);
        if (vault_authtype_buf.val == "APPROLE") {
          auth_type = APPROLE_AUTH_TYPE;
        }
        vault = vault_factory.get_vault_interface(vault_addr_buf.val, \
            secrets_path, 5, auth_type, vault_un_buf.val, vault_pw_buf.val);
      }
    }
  }

  inline void load_consul_info(KeyValueStoreInterface *kv) {
    if (kv) {
      // Check the commandline arguments for Consul Information
      std::string consul_add_key = "consul";
      std::string consul_cert_key = "consul.cert";
      std::string consul_token_key = "consul.token";
      StringBuffer consul_addr_buf;
      StringBuffer consul_cert_buf;
      StringBuffer consul_token_buf;
      if (kv->opt_exist(consul_add_key) && \
          kv->opt_exist(consul_cert_key) && \
          kv->opt_exist(consul_token_key)) {
        kv->get_opt(consul_add_key, consul_addr_buf);
        kv->get_opt(consul_cert_key, consul_cert_buf);
        kv->get_opt(consul_token_key, consul_token_buf);
        ApplicationProfile::set_consul_address(consul_addr_buf.val, 5, \
            consul_cert_buf.val, consul_token_buf.val);
      } else if (kv->opt_exist(consul_add_key) && \
          kv->opt_exist(consul_cert_key) && \
          kv->opt_exist(consul_token_key)) {
        kv->get_opt(consul_add_key, consul_addr_buf);
        kv->get_opt(consul_cert_key, consul_cert_buf);
        ApplicationProfile::set_consul_address(consul_addr_buf.val, 5, \
            consul_cert_buf.val);
      } else if (kv->opt_exist(consul_add_key)) {
        kv->get_opt(consul_add_key, consul_addr_buf);
        ApplicationProfile::set_consul_address(consul_addr_buf.val);
      }
    }
  }

  // Initialize the Profile
  inline void init() {
    // Check Env Variables for Consul Information
    const char *env_consul_value = std::getenv("AOSSL_CONSUL_ADDRESS");
    const char *env_consul_cert_value = std::getenv("AOSSL_CONSUL_SSL_CERT");
    const char *env_consul_token_value = std::getenv("AOSSL_CONSUL_ACL_TOKEN");
    if (env_consul_value && env_consul_cert_value && env_consul_token_value) {
      std::string consul_addr_str(env_consul_value);
      std::string consul_cert_str(env_consul_value);
      std::string consul_token_str(env_consul_value);
      ApplicationProfile::set_consul_address(consul_addr_str, 5, consul_cert_str, env_consul_value);
    } else if (env_consul_value && env_consul_cert_value) {
      std::string consul_addr_str(env_consul_value);
      std::string consul_cert_str(env_consul_value);
      ApplicationProfile::set_consul_address(consul_addr_str, 5, consul_cert_str);
    } else if (env_consul_value) {
      std::string consul_addr_str(env_consul_value);
      ApplicationProfile::set_consul_address(consul_addr_str);
    }

    // Check environment variables for vault information
    const char *env_vault_addr = std::getenv("AOSSL_VAULT_ADDRESS");
    const char *env_vault_cert = std::getenv("AOSSL_VAULT_SSL_CERT");
    const char *env_vault_authtype = std::getenv("AOSSL_VAULT_AUTH_TYPE");
    const char *env_vault_authun = std::getenv("AOSSL_VAULT_AUTH_UN");
    const char *env_vault_authpw = std::getenv("AOSSL_VAULT_AUTH_PW");
    if (env_vault_addr && env_vault_cert && env_vault_authtype && env_vault_authun && env_vault_authpw) {
      std::string vaddr(env_vault_addr);
      std::string cert(env_vault_cert);
      std::string secrets_path("/v1/secret/data/");
      std::string un(env_vault_authun);
      std::string pw(env_vault_authpw);
      std::string authtype_string(env_vault_authtype);
      int auth_type = BASIC_AUTH_TYPE;
      if (authtype_string == "APPROLE") {
        auth_type = APPROLE_AUTH_TYPE;
      }
      set_vault_address(vaddr, secrets_path, 5, cert, auth_type, un, pw);
    } else if (env_vault_addr && env_vault_authtype && env_vault_authun && env_vault_authpw) {
      std::string vaddr(env_vault_addr);
      std::string cert(env_vault_cert);
      std::string secrets_path("/v1/secret/data/");
      std::string un(env_vault_authun);
      std::string pw(env_vault_authpw);
      std::string authtype_string(env_vault_authtype);
      int auth_type = BASIC_AUTH_TYPE;
      if (authtype_string == "APPROLE") {
        auth_type = APPROLE_AUTH_TYPE;
      }
      set_vault_address(vaddr, secrets_path, 5, auth_type, un, pw);
    }

    // Check Env Variables for a Properties File
    const char *env_props_value = std::getenv("AOSSL_PROPS_FILE");
    if (env_props_value) {
      std::string props_file_str(env_props_value);
      ApplicationProfile::set_property_file(props_file_str);
    }

    // Check the commandline arguments
    load_consul_info(ApplicationProfile::get_cli());
    load_vault_info(ApplicationProfile::get_cli());
    if (ApplicationProfile::get_cli()) {
      // Check the commandline arguments for a properties file
      if (ApplicationProfile::get_cli()->opt_exist(std::string("props"))) {
        StringBuffer props_buf;
        ApplicationProfile::get_cli()->get_opt(std::string("props"), props_buf);
        ApplicationProfile::set_property_file(props_buf.val);
      }
    }
    // Check for possible property files
    props_file_name = "app.properties";
    if ((!(ApplicationProfile::get_props())) && exists_test(props_file_name)) {
      ApplicationProfile::set_property_file(props_file_name);
    }
    // Check the Properties file for the Consul Address
    load_consul_info(ApplicationProfile::get_props());
    load_vault_info(ApplicationProfile::get_props());
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

  ~TieredApplicationProfile() {}

  //! Load the configuration from the various sources
  inline void load_config() {
    // Iterate over the default values and pull
    // values from available sources with the same key
    for (std::pair<std::string, std::string> element : \
        KeyValueStore::get_opts()) {
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
    for (std::string secure_opt : secure_opt_keys) {
      // Load Vault Secret, if present
      load_vault_secret(vault, secure_opt);
      // Load Properties File values, if present
      load_config_value(ApplicationProfile::get_props(), secure_opt);
      // Load environment variables, if present
      load_environment_variable(secure_opt);
    }
  }

  //! Add a secure configuration value to search for
  //! These are prioritized in the following order:
  //! 1. Environment variables
  //! 2. Properties File values
  //! 3. Vault Secret Values
  inline void add_secure_opt(std::string& key) {
    secure_opt_keys.push_back(key);
  }

  //! Update the Vault connectivity information
  inline void set_vault_address(std::string& vaddr, std::string& secrets_path, \
      int tout, std::string& cert, int auth_type, std::string& un, std::string& pw) {
    if (vault) {
      delete vault;
      vault = nullptr;
    }
    vault = vault_factory.get_vault_interface(vaddr, secrets_path, 5, cert, auth_type, un, pw);
  }

  //! Update the Vault connectivity information
  inline void set_vault_address(std::string& vaddr, std::string& secrets_path, \
      int tout, int auth_type, std::string& un, std::string& pw) {
    if (vault) {
      delete vault;
      vault = nullptr;
    }
    vault = vault_factory.get_vault_interface(vaddr, secrets_path, 5, auth_type, un, pw);
  }
};

}  // namespace AOSSL

#endif  // AOSSL_PROFILE_INCLUDE_TIERED_APP_PROFILE_H_
