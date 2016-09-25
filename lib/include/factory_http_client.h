#include "factory/http_interface.h"
#include "http_admin.h"

#ifndef AOSSL_FACTORY_HTTP_CLIENT
#define AOSSL_FACTORY_HTTP_CLIENT

//! The HTTP Client Service Component Factory

//! The Service Component Factory tracks the HTTP Client
//! objects exposed by the framework and passes back
//! instances of interfaces.  This allows for the publicly exposed methods
//! to be independent of the implementations.
class HttpClientFactory
{
public:

  //! Create a new Service Component Factory
  HttpClientFactory() {}

  //! Delete a Service Component Factory
  ~HttpClientFactory() {}

  //! Get the HTTP Interface instance
  HttpInterface* get_http_interface() {return new HttpAdmin;}

};

#endif
