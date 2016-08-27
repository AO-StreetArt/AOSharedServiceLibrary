#include "factory/http_server_interface.h"
#include <vector>
#include <string>
#include <exception>
#include "factory/callbacks.h"
#include <iostream>

#ifndef AOSSL_HTTP_SERVER
#define AOSSL_HTTP_SERVER

typedef void (*HttpCallback)(struct evhttp_request*, void*);

void process_request(struct evhttp_request *req, void *arg);

class HttpServer: public HttpServerInterface
{
struct event_base *base;
std::unordered_map<std::string, CallbackInterface> callback_map;
std::string internal_url;
int internal_port;
public:
  HttpServer(std::string base_addr, int base_port);
  ~HttpServer();
  bool bind_callback(std::string uri, CallbackInterface func);
  void recv();
};

#endif
