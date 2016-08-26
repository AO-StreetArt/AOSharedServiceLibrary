#include "factory/http_server_interface.h"
#include <vector>
#include <string>

class HttpServer: public HttpServerInterface
{
struct event_base *base;
std::vector<struct *evhttp> http_addresses;
public:
  HttpServer();
  ~HttpServer();
  bool bind_callback(std::string url, int port, HttpCallback func);
  std::string recv();
};
