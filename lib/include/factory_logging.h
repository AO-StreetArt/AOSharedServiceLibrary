#include <string.h>
#include "factory/logging_interface.h"
#include "logging.h"

#ifndef AOSSL_FACTORY_LOGGING
#define AOSSL_FACTORY_LOGGING

//! The Logging Service Component Factory

//! The Service Component Factory tracks the Logging
//! objects exposed by the framework and passes back
//! instances of interfaces.  This allows for the publicly exposed methods
//! to be independent of the implementations.
class LoggingComponentFactory
{
public:

  //! Create a new Service Component Factory
  LoggingComponentFactory();

  //! Delete a Service Component Factory
  ~LoggingComponentFactory();

  //! Get a Logging Interface instance
  LoggingInterface* get_logging_interface( std::string initFileName ) {return new Logger( initFileName );}

};

#endif
