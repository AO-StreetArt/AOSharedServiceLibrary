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
#include <exception>

#include "aossl/core/include/buffers.h"

#ifndef AOSSL_UUID_INCLUDE_UUID_INTERFACE_H_
#define AOSSL_UUID_INCLUDE_UUID_INTERFACE_H_

namespace AOSSL {

//! UUID Admin

//! The UUID Admin is in charge of generating any Universally Unique
//! ID's that are required throughout program execution
class UuidInterface {
 public:
  virtual ~uuidInterface() {}
  //! Generate a new UUID and allocate memory for it.

  //! The method will generate on the means of generation present on your
  //! system.  In some cases, this may result in UUID's being generated that
  //! pose a security risk.  In this case, that fact will be clearly called out
  //! in the logs, and it is recommended that production systems are
  //! tested to ensure that UUID's are generated in a safe manner
  virtual StringBuffer* generate() = 0;
  //! Generate a new UUID in the pre-existing string buffer.

  //! The method will generate on the means of generation present on your
  //! system.  In some cases, this may result in UUID's being generated that
  //! pose a security risk.  In this case, that fact will be clearly called out
  //! in the logs, and it is recommended that production systems are
  //! tested to ensure that UUID's are generated in a safe manner
  virtual void generate(StringBuffer& buf) = 0;
};

}

#endif  // AOSSL_UUID_INCLUDE_UUID_INTERFACE_H_
