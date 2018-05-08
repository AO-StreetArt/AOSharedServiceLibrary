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
#include <iostream>

#include "buffers.h"
#include "kv_store_interface.h"

#ifndef AOSSL_CORE_INCLUDE_KV_STORE_H_
#define AOSSL_CORE_INCLUDE_KV_STORE_H_

namespace AOSSL {

//! Key Value Store

//! A Key-Value store accesses configuration values by keys
//! Interface which requires implementation
class KeyValueStore: public KeyValueStoreInterface {
  std::unordered_map<std::string, std::string> opts;
 public:
  KeyValueStore() {}
  virtual ~KeyValueStore() {}
  //! Does a key exist?
  inline bool opt_exist(std::string key) {
    auto search = opts.find(key);
    if (search != opts.end()) {
      return true;
    } else {
      return false;
    }
  }

  //! Get an option by key
  inline StringBuffer* get_opt(std::string key) {
    StringBuffer *ret_buf = new StringBuffer();
    get_opt(key, *ret_buf);
    return ret_buf;
  }

  //! Get an option by key
  inline void get_opt(std::string key, StringBuffer& val) {
    val.val.assign(opts[key]);
    val.success = true;
  }

  std::unordered_map<std::string, std::string> get_opts() {return opts;}

  //! Add an option
  void add_opt(const std::string& key, const std::string& value) {opts.emplace(key, value);}

  //! Set an option
  void set_opt(std::string& key, std::string& value) {opts[key] = value;}
};

}

#endif  // AOSSL_CORE_INCLUDE_KV_STORE_H_
