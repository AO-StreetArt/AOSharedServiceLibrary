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

#include <stdlib.h>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <unordered_map>

#include "aossl/core/include/kv_store.h"
#include "aossl/core/include/kv_store_interface.h"
#include "app_profile.h"
#include "safe_app_profile.h"

#ifndef AOSSL_SESSION_INCLUDE_BIASED_APP_SESSION_H_
#define AOSSL_SESSION_INCLUDE_BIASED_APP_SESSION_H_

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
  // Load a value from an environment variable
  inline void load_environment_variable(std::string& key) {
    const char *env_value = std::getenv(key.c_str());
    if (env_value) {
      std::string env_str(env_value);
      KeyValueStore::set_opt(key, env_str);
    }
  }
 public:
  TieredApplicationProfile(int argc, char* argv[]) : \
      SafeApplicationProfile(argc, argv) {}

  ~TieredApplicationProfile() {}

  //! Load the configuration from the various sources
  inline void load_config() {
    // Iterate over the default values and pull
    // values from available sources with the same key
    for (std::pair<std::string, std::string> element : KeyValueStore::get_opts()) {
      // Load Properties File values, if present
      load_config_value(ApplicationProfile::get_props(), element.first);
      // Load Consul Values, if present
      load_config_value(ApplicationProfile::get_consul(), element.first);
      // Load Environment variables
      load_environment_variable(element.first);
      // Load Commandline Values, if present
      load_config_value(ApplicationProfile::get_cli(), element.first);
    }
  }
};

}

#endif  // AOSSL_SESSION_INCLUDE_BIASED_APP_SESSION_H_
