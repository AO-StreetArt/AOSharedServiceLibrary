/*
MIT License Block

Copyright (c) 2016 Alex Barry

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// This sets up all of the components necessary for the service and runs the
// main loop for the application.

#include "include/app_log.h"

LoggingCategoryInterface *config_logging = NULL;
LoggingCategoryInterface *redis_logging = NULL;
LoggingCategoryInterface *main_logging = NULL;
LoggingCategoryInterface *uuid_logging = NULL;

void start_logging_submodules() {
  if (!config_logging) config_logging = logging->get_category("configuration");
  if (!redis_logging) redis_logging = logging->get_category("redis");
  if (!main_logging) main_logging = logging->get_category("main");
}

void shutdown_logging_submodules() {
  if (!config_logging) {
    logging->debug("Config Logging delete called without initialized object");
  } else {
    logging->debug("Configuration Logging Module delete called");
    delete config_logging;
  }
  if (!redis_logging) {
    logging->debug("Document Logging delete without initialized object");
  } else {
    logging->debug("Document Manager Logging Module delete called");
    delete redis_logging;
  }
  if (!main_logging) {
    logging->debug("Main Logging delete without initialized object");
  } else {
    logging->debug("Main Logging Module delete called");
    delete main_logging;
  }
}
