//Interface for HTTP Server Objects

#include <evhttp.h>
#include <string>
#include "callbacks.h"

#ifndef HTTP_SERVER_INTERFACE
#define HTTP_SERVER_INTERFACE

//! This is a basic HTTP Server that can be used to build a RESTful API.

//! The HTTP Server can bind static API methods to individual callbacks,
//! and dynamic API methods can go to a default callback.  Used to build
//! RESTful API's
class HttpServerInterface
{
public:

  //! Bind a callback for a static API Method
  virtual bool bind_callback(std::string uri, CallbackInterface func) = 0;

  //! Bind a callback for a dynamic API Method
  virtual bool bind_default_callback(CallbackInterface func) = 0;

  //! Blocking call for waiting for incoming API Calls
  virtual void recv() = 0;
  
  virtual ~HttpServerInterface() {}
};

#endif
