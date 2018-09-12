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

#ifndef AOSSL_CORE_INCLUDE_BUFFERS_H_
#define AOSSL_CORE_INCLUDE_BUFFERS_H_

namespace AOSSL {

//! A base buffer
struct Buffer {
  //! Success flag
  bool success = false;

  //! Error Message
  std::string err_msg;
};

//! A Structure for holding a single value
struct StringBuffer: public Buffer {
  //! Value stored
  std::string val;
};

//! A Structure for storing a Key-Value pair
struct KvBuffer: public StringBuffer {
  //! Key of the pair
  std::string key;
};

//! A Structure for storing an SSL Certificate
struct SslCertificateBuffer: public Buffer {
  //! Certificate String
  std::string certificate;
  //! Issuing CA String
  std::string issuing_ca;
  //! CA Chain String
  std::string ca_chain;
  //! Private Key String
  std::string private_key;
  //! Private Key Type
  std::string private_key_type;
  //! Serial Number
  std::string serial_number;
  //! Amount of time the certificate will be valid
  int lease_duration = 0;
};

}  // namespace AOSSL
#endif  // AOSSL_CORE_INCLUDE_BUFFERS_H_
