#include "include/app_log.h"

LoggingCategoryInterface *config_logging = NULL;
LoggingCategoryInterface *consul_logging = NULL;
LoggingCategoryInterface *request_logging = NULL;
LoggingCategoryInterface *main_logging = NULL;
LoggingCategoryInterface *uuid_logging = NULL;

void start_logging_submodules()
{
  if (!consul_logging) {uuid_logging = logging->get_category("uuid");}
  if (!config_logging) {config_logging = logging->get_category("configuration");}
  if (!request_logging) {request_logging = logging->get_category("request");}
  if (!main_logging) {main_logging = logging->get_category("main");}
  if (!uuid_logging) {uuid_logging = logging->get_category("uuid");}
}

void shutdown_logging_submodules()
{
  if (!request_logging)
  {
    logging->debug("Request Logging Module delete called without initialized object");
  }
  else
  {
    logging->debug("Request Logging Module delete called");
    delete request_logging;
  }
  if (!config_logging)
  {
    logging->debug("Configuration Logging Module delete called without initialized object");
  }
  else
  {
    logging->debug("Configuration Logging Module delete called");
    delete config_logging;
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
  if (!main_logging)
  {
    logging->debug("Main Logging Module delete called without initialized object");
  }
  else
  {
    logging->debug("Main Logging Module delete called");
    delete main_logging;
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
}
