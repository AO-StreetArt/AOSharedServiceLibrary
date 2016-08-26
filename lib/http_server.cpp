#include "include/http_server.h"

HttpServer::HttpServer()
{
  base = NULL;
  base = event_init();
}

HttpServer::~HttpServer()
{
  // if (base != NULL) delete base;
}

bool HttpServer::bind_callback(std::string url, int port, HttpCallback func)
{
  struct evhttp *httpd;
  httpd = evhttp_new(base);
  if (httpd == NULL) return false;
  if (evhttp_bind_socket(httpd, url.c_str(), port) != 0) return false;
  evhttp_set_gencb(httpd, func, NULL);
  return true;
}

void HttpServer::recv()
{
  event_base_dispatch(base);
}
