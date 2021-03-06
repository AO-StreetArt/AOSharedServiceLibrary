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

#include <vector>
#include <string>
#include "aossl/core/include/kv_store.h"
#include "aossl/commandline/include/cli.h"

#ifndef AOSSL_COMMANDLINE_INCLUDE_FACTORY_CLI_H_
#define AOSSL_COMMANDLINE_INCLUDE_FACTORY_CLI_H_

namespace AOSSL {

//! The Command Line Interpreter Service Component Factory

//! The Service Component Factory tracks the CLI
//! objects exposed by the framework and passes back
//! instances of interfaces.  This allows for the publicly exposed methods
//! to be independent of the implementations.
class CommandLineInterpreterFactory {
 public:
  //! Create a new Service Component Factory
  CommandLineInterpreterFactory() {}

  //! Delete a Service Component Factory
  ~CommandLineInterpreterFactory() {}

  //! Get the Command Line Interface instance
  KeyValueStore* get_command_line_interface(int argc, char* argv[]) {
    return new CommandLineInterpreter( argc, argv );
  }

  //! Get the Command Line Interface instance
  KeyValueStore* get_command_line_interface(\
      const std::vector<std::string>& args) {
    return new CommandLineInterpreter( args );
  }
};

}  // namespace AOSSL

#endif  // AOSSL_COMMANDLINE_INCLUDE_FACTORY_CLI_H_
