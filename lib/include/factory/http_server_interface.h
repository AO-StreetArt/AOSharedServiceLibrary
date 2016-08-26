//Interface for HTTP Server Objects

#include <evhttp.h>

typedef void (*HttpCallback)(struct evhttp_request*, void*);

class HttpServerInterface
{
public:
  virtual bool bind_callback(std::string url, int port, HttpCallback func) = 0;
  virtual std::string recv() = 0;
};
