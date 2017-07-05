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

// This implements interfaces on callbacks in the form of function pointers,
// and structs that are passed to them on being called
// These function pointers are used as callbacks in the HTTP Server

#ifndef AOSSL_HTTP_SERVER_INCLUDE_CALLBACKS_H_
#define AOSSL_HTTP_SERVER_INCLUDE_CALLBACKS_H_

// Constants defining types

const int UNKNOWN = 111;

const int HTTP_GET = 15;
const int HTTP_PUT = 16;
const int HTTP_POST = 17;
const int HTTP_DELETE = 18;

const int DB_CREATE = 19;
const int DB_SAVE = 20;
const int DB_GET = 21;
const int DB_DELETE = 22;

// Constants defining error codes
const int NOERROR = 0;
const int COUCHBASE_BADREQUEST = 801;

//! A struct that gets passed to callbacks to transmit errors
struct RequestError {
  //! A numerical error code
  int err_code;

  //! An Error message
  std::string err_message;
  RequestError() {err_code = NOERROR; err_message = "";}
};

//! A struct that gets passed to callbacks
struct Request {
  //! Used to store original request data
  std::string req_data;

  //! Used to store the address on the original request
  std::string req_addr;

  //! Used to store the request type
  int req_type;

  //! Used to store any error messages from the request
  RequestError *req_err;

  //! Used to store response data from the request
  std::string resp_data;

  //! Constructor
  inline Request() {
    req_data = "";
    req_type = UNKNOWN;
    req_err = new RequestError();
    resp_data = "";
  }
  ~Request() {delete req_err;}
};

typedef std::string (*CallbackInterface)(struct Request*);

#endif  // AOSSL_HTTP_SERVER_INCLUDE_CALLBACKS_H_
