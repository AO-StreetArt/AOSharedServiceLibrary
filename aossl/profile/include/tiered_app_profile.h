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
        if (!(buf.val.empty())) {
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
        } else {

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
  // Initialize the Profile
  inline void init() {
    // Check the CLI and Env Variables for Consul Address and Properties File
    const char *env_consul_value = std::getenv("AOSSL_CONSUL_ADDRESS");
    if (env_consul_value) {
      std::string consul_addr_str(env_consul_value);
      ApplicationProfile::set_consul_address(consul_addr_str);
    }
    if (ApplicationProfile::get_cli()) {
      if (ApplicationProfile::get_cli()->opt_exist(std::string("consul"))) {
        StringBuffer consul_buf;
        ApplicationProfile::get_cli()->get_opt(std::string("consul"), \
          consul_buf);
        ApplicationProfile::set_consul_address(consul_buf.val);
      }
    }
    const char *env_props_value = std::getenv("AOSSL_PROPS_FILE");
    if (env_props_value) {
      std::string props_file_str(env_props_value);
      ApplicationProfile::set_property_file(props_file_str);
    }
    if (ApplicationProfile::get_cli()) {
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
    if (ApplicationProfile::get_props()) {
        if (ApplicationProfile::get_props()->opt_exist(std::string("consul"))) {
          StringBuffer pconsul_buf;
          ApplicationProfile::get_cli()->get_opt(std::string("consul"), \
            pconsul_buf);
          ApplicationProfile::set_consul_address(pconsul_buf.val);
        }
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
  }
};

}  // namespace AOSSL

#endif  // AOSSL_PROFILE_INCLUDE_TIERED_APP_PROFILE_H_
