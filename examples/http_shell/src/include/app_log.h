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

#include "aossl/logging/include/logging_interface.h"

#ifndef EXAMPLES_HTTP_SHELL_SRC_INCLUDE_APP_LOG_H_
#define EXAMPLES_HTTP_SHELL_SRC_INCLUDE_APP_LOG_H_

extern LoggingCategoryInterface *consul_logging;
extern LoggingCategoryInterface *config_logging;
extern LoggingCategoryInterface *request_logging;
extern LoggingCategoryInterface *uuid_logging;
extern LoggingCategoryInterface *main_logging;

void start_logging_submodules();
void shutdown_logging_submodules();

#endif  // EXAMPLES_HTTP_SHELL_SRC_INCLUDE_APP_LOG_H_
