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
