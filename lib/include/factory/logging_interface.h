#include <log4cpp/Category.hh>
#include <string>

#ifndef LOGGING_INTERFACE
#define LOGGING_INTERFACE

class LoggingCategoryInterface
{
public:
  virtual ~LoggingCategoryInterface() {}

  //Exposures that log to the sepcified category

  //! Log at a debug level to the root category
  virtual void debug(std::string msg) = 0;

  //! Log at an error level to the root category
  virtual void error(std::string msg) = 0;

  //! Log at an info level to the root category
  virtual void info(std::string msg) = 0;

  //! Log at an debug level to the root category
  virtual void debug(const char * msg) = 0;

  //! Log at an error level to the root category
  virtual void error(const char * msg) = 0;

  //! Log at an info level to the root category
  virtual void info(const char * msg) = 0;
};

class LoggingInterface
{
public:

  virtual ~LoggingInterface() {}

  //Exposures that log to the root category

  //! Log at a debug level to the root category
  virtual void debug(std::string msg) = 0;

  //! Log at an error level to the root category
  virtual void error(std::string msg) = 0;

  //! Log at an info level to the root category
  virtual void info(std::string msg) = 0;

  //! Log at an debug level to the root category
  virtual void debug(const char * msg) = 0;

  //! Log at an error level to the root category
  virtual void error(const char * msg) = 0;

  //! Log at an info level to the root category
  virtual void info(const char * msg) = 0;

  //! Pull down different categories by name
  virtual LoggingCategoryInterface* get_category(std::string name) = 0;
};

//Define the globals needed

//! The global logging pointer that is instantiated and deleted at startup/shutdown, respectively
extern LoggingInterface *logging;
extern LoggingCategoryInterface *consul_logging;
extern LoggingCategoryInterface *cb_logging;
extern LoggingCategoryInterface *http_logging;
extern LoggingCategoryInterface *uuid_logging;
extern LoggingCategoryInterface *redis_logging;
extern LoggingCategoryInterface *zmq_logging;

//! Shutdown the internal framework logging subcategories

//! Delete the logging categories that are created internally in the framework
//! Every application should call this method on closing
void shutdown_framework_logging()
{
  if (!cb_logging)
  {
    logging->debug("Couchbase Logging Module delete called without initialized object");
  }
  else
  {
    delete cb_logging;
  }
  if (!zmq_logging)
  {
    logging->debug("ZMQ Logging Module delete called without initialized object");
  }
  else
  {
    delete zmq_logging;
  }
  if (!consul_logging)
  {
    logging->debug("Consul Logging Module delete called without initialized object");
  }
  else
  {
    delete consul_logging;
  }
  if (!http_logging)
  {
    logging->debug("HTTP Logging Module delete called without initialized object");
  }
  else
  {
    delete http_logging;
  }
  if (!uuid_logging)
  {
    logging->debug("UUID Logging Module delete called without initialized object");
  }
  else
  {
    delete uuid_logging;
  }
  if (!redis_logging)
  {
    logging->debug("Redis Logging Module delete called without initialized object");
  }
  else
  {
    delete redis_logging;
  }
}

#endif
