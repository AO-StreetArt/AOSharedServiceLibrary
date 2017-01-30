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

//Interface for HTTP Server Objects

#include <evhttp.h>
#include <string>
#include "callbacks.h"

#ifndef HTTP_SERVER_INTERFACE
#define HTTP_SERVER_INTERFACE

//! This is a basic HTTP Server that can be used to build a RESTful API.

//! The HTTP Server can bind static API methods to individual callbacks,
//! and dynamic API methods can go to a default callback.  Used to build
//! RESTful API's
class HttpServerInterface
{
public:

  //! Bind a callback for a static API Method
  virtual bool bind_callback(std::string uri, CallbackInterface func) = 0;

  //! Bind a callback for a dynamic API Method
  virtual bool bind_default_callback(CallbackInterface func) = 0;

  //! Blocking call for waiting for incoming API Calls
  virtual void recv() = 0;

  virtual ~HttpServerInterface() {}
};

#endif
