#ifndef AOSSL_APP_LOGGING
#define AOSSL_APP_LOGGING

#include "aossl/logging/include/logging_interface.h"

extern LoggingCategoryInterface *consul_logging;
extern LoggingCategoryInterface *config_logging;
extern LoggingCategoryInterface *request_logging;
extern LoggingCategoryInterface *uuid_logging;
extern LoggingCategoryInterface *main_logging;

void start_logging_submodules();
void shutdown_logging_submodules();

#endif
