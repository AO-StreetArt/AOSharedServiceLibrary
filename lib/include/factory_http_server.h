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
