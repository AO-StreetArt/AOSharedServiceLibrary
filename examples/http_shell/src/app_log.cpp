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

#include "include/app_log.h"

LoggingCategoryInterface *config_logging = NULL;
LoggingCategoryInterface *consul_logging = NULL;
LoggingCategoryInterface *request_logging = NULL;
LoggingCategoryInterface *main_logging = NULL;
LoggingCategoryInterface *uuid_logging = NULL;

void start_logging_submodules() {
  if (!consul_logging) uuid_logging = logging->get_category("uuid");
  if (!config_logging) config_logging = logging->get_category("configuration");
  if (!request_logging) request_logging = logging->get_category("request");
  if (!main_logging) main_logging = logging->get_category("main");
  if (!uuid_logging) uuid_logging = logging->get_category("uuid");
}

void shutdown_logging_submodules() {
  if (!request_logging) {
    logging->debug("Request Log Module delete without initialized object");
  } else {
    logging->debug("Request Logging Module delete called");
    delete request_logging;
  }
  if (!config_logging) {
    logging->debug("Config Log Module delete without initialized object");
  } else {
    logging->debug("Configuration Logging Module delete called");
    delete config_logging;
  }
  if (!consul_logging) {
    logging->debug("Consul Log Module delete without initialized object");
  } else {
    logging->debug("Consul Logging Module delete called");
    delete consul_logging;
  }
  if (!main_logging) {
    logging->debug("Main Log Module delete without initialized object");
  } else {
    logging->debug("Main Logging Module delete called");
    delete main_logging;
  }
  if (!uuid_logging) {
    logging->debug("UUID Log Module delete without initialized object");
  } else {
    logging->debug("UUID Logging Module delete called");
    delete uuid_logging;
  }
}
