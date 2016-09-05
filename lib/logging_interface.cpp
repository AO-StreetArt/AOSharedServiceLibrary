#include "include/factory/logging_interface.h"

LoggingInterface *logging = NULL;

LoggingCategoryInterface *consul_logging = NULL;

LoggingCategoryInterface *cb_logging = NULL;

LoggingCategoryInterface *http_logging = NULL;

LoggingCategoryInterface *uuid_logging = NULL;

LoggingCategoryInterface *redis_logging = NULL;

LoggingCategoryInterface *zmq_logging = NULL;
