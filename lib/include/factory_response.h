#include "factory/response_interface.h"
#include "response.h"

#ifndef AOSSL_FACTORY_RESPONSE
#define AOSSL_FACTORY_RESPONSE

//! The ZMQ Service Component Factory

//! The Service Component Factory tracks the ZMQ
//! objects exposed by the framework and passes back
//! instances of interfaces.  This allows for the publicly exposed methods
//! to be independent of the implementations.
class ResponseFactory
{
public:

  //! Create a new Service Component Factory
  ResponseFactory() {}

  //! Delete a Service Component Factory
  ~ResponseFactory() {}

  //! Get an application response interface instance
  ApplicationResponseInterface* get_application_response_interface() {return new ApplicationResponse;}

};

#endif
