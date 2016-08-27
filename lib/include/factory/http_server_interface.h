//Interface for HTTP Server Objects

#include <evhttp.h>
#include <string>
#include "callbacks.h"

#ifndef HTTP_SERVER_INTERFACE
#define HTTP_SERVER_INTERFACE

class HttpServerInterface
{
public:
  virtual bool bind_callback(std::string uri, CallbackInterface func) = 0;
  virtual void recv() = 0;
};

#endif
