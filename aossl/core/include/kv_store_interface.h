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

#include "aossl/core/include/buffers.h"

#ifndef AOSSL_CORE_INCLUDE_KV_STORE_INTERFACE_H_
#define AOSSL_CORE_INCLUDE_KV_STORE_INTERFACE_H_

namespace AOSSL {

//! Key Value Store

//! A Key-Value store accesses configuration values by keys
//! Interface which requires implementation
class KeyValueStoreInterface {
 public:
  virtual ~KeyValueStoreInterface() {}
  //! Does a key exist?
  virtual bool opt_exist(std::string key) = 0;

  //! Get an option by key
  virtual StringBuffer* get_opt(std::string key) = 0;

  //! Get an option by key
  virtual void get_opt(std::string key, StringBuffer& val) = 0;

  //! Re-load configuration
  virtual void load_config() = 0;
};

}  // namespace AOSSL

#endif  // AOSSL_CORE_INCLUDE_KV_STORE_INTERFACE_H_
