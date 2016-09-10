#include "include/factory/logging_interface.h"

LoggingInterface *logging = NULL;

LoggingCategoryInterface *consul_logging = NULL;

LoggingCategoryInterface *cb_logging = NULL;

LoggingCategoryInterface *http_logging = NULL;

LoggingCategoryInterface *uuid_logging = NULL;

LoggingCategoryInterface *redis_logging = NULL;

LoggingCategoryInterface *zmq_logging = NULL;

void shutdown_framework_logging()
{
  if (!cb_logging)
  {
    logging->debug("Couchbase Logging Module delete called without initialized object");
  }
  else
  {
    logging->debug("Couchbase Logging Module delete called");
    delete cb_logging;
  }
  if (!zmq_logging)
  {
    logging->debug("ZMQ Logging Module delete called without initialized object");
  }
  else
  {
    logging->debug("ZMQ Logging Module delete called");
    delete zmq_logging;
  }
  if (!consul_logging)
  {
    logging->debug("Consul Logging Module delete called without initialized object");
  }
  else
  {
    logging->debug("Consul Logging Module delete called");
    delete consul_logging;
  }
  if (!http_logging)
  {
    logging->debug("HTTP Logging Module delete called without initialized object");
  }
  else
  {
    logging->debug("HTTP Logging Module delete called");
    delete http_logging;
  }
  if (!uuid_logging)
  {
    logging->debug("UUID Logging Module delete called without initialized object");
  }
  else
  {
    logging->debug("UUID Logging Module delete called");
    delete uuid_logging;
  }
  if (!redis_logging)
  {
    logging->debug("Redis Logging Module delete called without initialized object");
  }
  else
  {
    logging->debug("Redis Logging Module delete called");
    delete redis_logging;
  }
}
