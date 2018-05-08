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

#include <string>
#include <iterator>

#include "aossl/core/include/kv_store.h"
#include "aossl/core/include/buffers.h"
#include <vector>

#ifndef AOSSL_COMMANDLINE_INCLUDE_CLI_H_
#define AOSSL_COMMANDLINE_INCLUDE_CLI_H_

namespace AOSSL {

class CommandLineInterpreter: public KeyValueStore {
  std::string name;
  void process_arg(const std::string& arg);
 public:
  // Create a new Command Line Interpreter

  // Here we create a new interpreter by passing in the two
  // arguments from the main method, int argc & char* argv[].
  // This parses arguments passed in the form:
  // arg_key=arg_val
  CommandLineInterpreter(int argc, char* argv[]) : KeyValueStore() {
    name = argv[0];
    for (int i = 1; i < argc; i++) {
      std::string full_line(argv[i]);
      process_arg(full_line);
    }
  }
  // Create a new Command Line Interpreter

  // Here we create a new interpreter by passing in the two
  // arguments from the main method, int argc & char* argv[].
  // This parses arguments passed in the form:
  // arg_key=arg_val
  CommandLineInterpreter(const std::vector<std::string>& args) : KeyValueStore() {
    name = args[0];
    for (std::string elt : args) {
      process_arg(elt);
    }
  }

  ~CommandLineInterpreter() {}

  std::string get_name() {return name;}
  void load_config() {}
};

}

#endif  // AOSSL_COMMANDLINE_INCLUDE_CLI_H_
