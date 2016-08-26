#include "include/http_server.h"

HttpServer::HttpServer()
{
  base = NULL;
  base = event_init();
  if (base == NULL) return -1;
}

HttpServer::~HttpServer()
{
  for(std::vector<struct *evhttp>::size_type i = 0; i != http_addresses.size(); i++) {
    delete http_addresses[i];
  }
  if (base != NULL) delete base;
}

bool HttpServer::bind_callback(std::string url, int port, HttpCallback func)
{
  struct evhttp *httpd;
  httpd = evhttp_new(base);
  if (httpd == NULL) return -1;
  if (evhttp_bind_socket(httpd, url, port) != 0) return -1;
  http_addresses.push_back(httpd);
  evhttp_set_gencb(httpd, func, NULL);
}

std::string HttpServer::recv()
{
  event_base_dispatch(base);
}
