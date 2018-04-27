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

#include <algorithm>
#include <stdlib.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

#include "aossl/core/include/kv_store.h"
#include "aossl/core/include/kv_store_interface.h"
#include "app_profile.h"
#include "safe_app_profile.h"

#ifndef AOSSL_SESSION_INCLUDE_TIERED_APP_PROFILE_H_
#define AOSSL_SESSION_INCLUDE_TIERED_APP_PROFILE_H_

namespace AOSSL {

//! Biased Application Session which implements tiered configuration.

//! Tiered Configuration prioritizes config values in the following order:
//! 1. Command Line Arguments
//! 2. Environment variables
//! 3. Consul Key-Value store variables
//! 4. Properties File values
//! 5. Default values
class TieredApplicationProfile: public SafeApplicationProfile{
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
      std::string query_key;
      // Prefix the key with the application & profile name
      // to ensure we get unique values for different apps
      if (!(ApplicationProfile::get_app_name().empty() && ApplicationProfile::get_profile_name().empty())) {
        query_key = ApplicationProfile::get_app_name() + std::string("/") + \
            ApplicationProfile::get_profile_name() + std::string("/");
      }
      query_key = query_key + key;
      if (kv->opt_exist(query_key)) {
        AOSSL::StringBuffer buf;
        kv->get_opt(query_key, buf);
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
        ApplicationProfile::get_consul()->base64_decode_by_reference(parsed_buffer.val, decoded_buffer);
        KeyValueStore::set_opt(key, decoded_buffer.val);
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
    // Get the environment variable
    const char *env_value = std::getenv(key.c_str());
    if (env_value) {
      std::string env_str(env_value);
      KeyValueStore::set_opt(key, env_str);
    }
  }
 public:
  TieredApplicationProfile(int argc, char* argv[]) : \
      SafeApplicationProfile(argc, argv) {}
  TieredApplicationProfile(int argc, char* argv[], std::string app_name, \
      std::string prof_name) : SafeApplicationProfile(argc, argv, app_name, prof_name) {}
  TieredApplicationProfile(std::string app_name, std::string prof_name) : \
      SafeApplicationProfile(app_name, prof_name) {}

  ~TieredApplicationProfile() {}

  //! Load the configuration from the various sources
  inline void load_config() {
    // Iterate over the default values and pull
    // values from available sources with the same key
    for (std::pair<std::string, std::string> element : KeyValueStore::get_opts()) {
      // Load Properties File values, if present
      load_config_value(ApplicationProfile::get_props(), element.first);
      // Load Consul Values, if present
      load_consul_value(ApplicationProfile::get_consul(), element.first);
      // Load Environment variables
      load_environment_variable(element.first);
      // Load Commandline Values, if present
      load_config_value(ApplicationProfile::get_cli(), element.first);
    }
  }
};

}

#endif  // AOSSL_SESSION_INCLUDE_TIERED_APP_PROFILE_H_
