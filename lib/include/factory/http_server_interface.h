//Interface for HTTP Server Objects

#include <evhttp.h>

#ifndef HTTP_SERVER_INTERFACE
#define HTTP_SERVER_INTERFACE

typedef void (*HttpCallback)(struct evhttp_request*, void*);

class HttpServerInterface
{
public:
  virtual bool bind_callback(std::string url, int port, HttpCallback func) = 0;
  virtual std::string recv() = 0;
};

#endif
