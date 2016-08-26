#include "factory/http_server_interface.h"
#include <vector>
#include <string>

#ifndef AOSSL_HTTP_SERVER
#define AOSSL_HTTP_SERVER

class HttpServer: public HttpServerInterface
{
struct event_base *base;
public:
  HttpServer();
  ~HttpServer();
  bool bind_callback(std::string url, int port, HttpCallback func);
  std::string recv();
};

#endif
