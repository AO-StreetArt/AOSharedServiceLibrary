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

#include <string.h>
#include <string>
#include <vector>
#include "aossl/core/include/kv_store_interface.h"
#include "aossl/vault/include/vault_admin.h"


#ifndef AOSSL_VAULT_INCLUDE_FACTORY_VAULT_H_
#define AOSSL_VAULT_INCLUDE_FACTORY_VAULT_H_

namespace AOSSL {

//! The Consul Service Component Factory

//! The Service Component Factory tracks the Consul
//! objects exposed by the framework and passes back
//! instances of interfaces.  This allows for the publicly exposed methods
//! to be independent of the implementations.
class VaultComponentFactory {
 public:
  //! Create a new Service Component Factory
  VaultComponentFactory() {}

  //! Delete a Service Component Factory
  ~VaultComponentFactory() {}

  //! Get a Consul Interface instance
  inline KeyValueStoreInterface* get_vault_interface(std::string& vaddr, \
      std::string& secrets_path, int tout, std::string& cert, \
      int auth_type, std::string& un, std::string& pw) {
    return new VaultAdmin(vaddr, secrets_path, tout, cert, auth_type, un, pw);
  }

  inline KeyValueStoreInterface* get_vault_interface(std::string& vaddr, \
      std::string& secrets_path, int tout, int auth_type, \
      std::string& un, std::string& pw) {
    return new VaultAdmin(vaddr, secrets_path, tout, auth_type, un, pw);
  }

};

}  // namespace AOSSL

#endif  // AOSSL_VAULT_INCLUDE_FACTORY_VAULT_H_
