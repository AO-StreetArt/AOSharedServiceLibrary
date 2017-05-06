#ifndef IVAN_LOGGING
#define IVAN_LOGGING

#include "aossl/logging/include/logging_interface.h"

extern LoggingCategoryInterface *config_logging;
extern LoggingCategoryInterface *redis_logging;
extern LoggingCategoryInterface *main_logging;

void start_logging_submodules();
void shutdown_logging_submodules();

#endif
