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

#include "factory/http_server_interface.h"
#include "http_server.h"
#include <string.h>

#ifndef AOSSL_FACTORY_HTTP_SERVER
#define AOSSL_FACTORY_HTTP_SERVER

//! The HTTP Server Service Component Factory

//! The Service Component Factory tracks the HTTP Server
//! objects exposed by the framework and passes back
//! instances of interfaces.  This allows for the publicly exposed methods
//! to be independent of the implementations.
class HttpServerFactory
{
public:

  //! Create a new Service Component Factory
  HttpServerFactory() {}

  //! Delete a Service Component Factory
  ~HttpServerFactory() {}

  HttpServerInterface* get_http_server_interface(std::string base_addr, int base_port) {return new HttpServer(base_addr, base_port);}

};

#endif
