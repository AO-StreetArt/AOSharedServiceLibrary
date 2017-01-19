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
#include <vector>
#include <string>
#include <exception>
#include "factory/callbacks.h"
#include <iostream>
#include <unordered_map>

#ifndef AOSSL_HTTP_SERVER
#define AOSSL_HTTP_SERVER

extern std::unordered_map<std::string, CallbackInterface> callback_map;

typedef void (*HttpCallback)(struct evhttp_request*, void*);

void process_request(struct evhttp_request *req, void *arg);

class HttpServer: public HttpServerInterface
{
struct event_base *base;
std::string internal_url;
int internal_port;
public:
  HttpServer(std::string base_addr, int base_port);
  ~HttpServer();
  bool bind_callback(std::string uri, CallbackInterface func);
  bool bind_default_callback(CallbackInterface func);
  void recv();
};

#endif
