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

#include <string>
#include <unordered_map>

#include "aossl/core/include/kv_store.h"
#include "app_profile.h"

#ifndef AOSSL_SESSION_INCLUDE_SAFE_APP_PROFILE_H_
#define AOSSL_SESSION_INCLUDE_SAFE_APP_PROFILE_H_

namespace AOSSL {

//! Application Session which accepts default configuration values.

//! Accepts a dictionary of default values
//! and exposes them as a KV Store
class SafeApplicationProfile: public ApplicationProfile, public KeyValueStore{
 public:
  SafeApplicationProfile(int argc, char* argv[]) : \
      ApplicationProfile(argc, argv) {}
  SafeApplicationProfile(int argc, char* argv[], std::string app_name, \
      std::string prof_name) : ApplicationProfile(argc, argv, app_name, prof_name) {}
  SafeApplicationProfile(const std::vector<std::string>& args) : \
      ApplicationProfile(args) {}
  SafeApplicationProfile(const std::vector<std::string>& args, std::string app_name, \
      std::string prof_name) : ApplicationProfile(args, app_name, prof_name) {}
  SafeApplicationProfile(std::string app_name, std::string prof_name) : \
      ApplicationProfile(app_name, prof_name) {}
  virtual ~SafeApplicationProfile() {}
};

}

#endif  // AOSSL_SESSION_INCLUDE_SAFE_APP_PROFILE_H_
