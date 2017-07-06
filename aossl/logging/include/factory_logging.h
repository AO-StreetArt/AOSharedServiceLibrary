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

#ifndef AOSSL_LOGGING_INCLUDE_FACTORY_LOGGING_H_
#define AOSSL_LOGGING_INCLUDE_FACTORY_LOGGING_H_

#include <string.h>
#include "logging_interface.h"
#include "logging.h"

//! The Logging Service Component Factory

//! The Service Component Factory tracks the Logging
//! objects exposed by the framework and passes back
//! instances of interfaces.  This allows for the publicly exposed methods
//! to be independent of the implementations.
class LoggingComponentFactory {
 public:
  //! Create a new Service Component Factory
  LoggingComponentFactory() {}

  //! Delete a Service Component Factory
  ~LoggingComponentFactory() {}

  //! Get a Logging Interface instance
  inline LoggingInterface* get_logging_interface( std::string initFileName ) {
    return new Logger( initFileName );
  }
};

#endif  // AOSSL_LOGGING_INCLUDE_FACTORY_LOGGING_H_
