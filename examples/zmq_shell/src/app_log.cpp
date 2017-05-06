#include "include/app_log.h"

LoggingCategoryInterface *config_logging = NULL;
LoggingCategoryInterface *redis_logging = NULL;
LoggingCategoryInterface *main_logging = NULL;
LoggingCategoryInterface *uuid_logging = NULL;

void start_logging_submodules()
{
  if (!config_logging) {config_logging = logging->get_category("configuration");}
  if (!redis_logging) {redis_logging = logging->get_category("redis");}
  if (!main_logging) {main_logging = logging->get_category("main");}
}

void shutdown_logging_submodules()
{
  if (!config_logging)
  {
    logging->debug("Configuration Logging Module delete called without initialized object");
  }
  else
  {
    logging->debug("Configuration Logging Module delete called");
    delete config_logging;
  }
  if (!redis_logging)
  {
    logging->debug("Document Manager Logging Module delete called without initialized object");
  }
  else
  {
    logging->debug("Document Manager Logging Module delete called");
    delete redis_logging;
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
}
