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
#include "aossl/core/include/kv_store_interface.h"

#ifndef AOSSL_VAULT_INCLUDE_VAULT_INTERFACE_H_
#define AOSSL_VAULT_INCLUDE_VAULT_INTERFACE_H_

namespace AOSSL {

//! Key Value Store

//! A Key-Value store accesses configuration values by keys
//! Interface which requires implementation
class VaultInterface: public KeyValueStoreInterface {
 public:
  virtual ~VaultInterface() {}
  //! Generate an SSL Certificate
  virtual void gen_ssl_cert(std::string& role_name, std::string& common_name, \
      SslCertificateBuffer& cert_buf) = 0;
  //! Generate a Consul ACL Token
  virtual void gen_consul_token(std::string& role_name, \
      StringBuffer& token_buf) = 0;
};

}  // namespace AOSSL

#endif  // AOSSL_VAULT_INCLUDE_VAULT_INTERFACE_H_
