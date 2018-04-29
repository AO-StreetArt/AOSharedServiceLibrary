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

#include "include/cli.h"

void AOSSL::CommandLineInterpreter::process_arg(const std::string& arg) {
  std::size_t pos = arg.find("=");

  // We have a key-value pair
  if (pos != std::string::npos) {
    std::string key = arg.substr(0, pos);
    std::string value = arg.substr(pos+1);
    KeyValueStore::set_opt(key, value);
  } else {
    // We have an opt flag, and these can be driven by opt_exist directly
    std::string key;
    key.assign(arg);
    std::string value = "N/A";
    KeyValueStore::set_opt(key, value);
  }
}

// Constructor
AOSSL::CommandLineInterpreter::CommandLineInterpreter(int argc, char* argv[]) {
  name = argv[0];
  for (int i = 1; i < argc; i++) {
    std::string full_line(argv[i]);
    process_arg(full_line);
  }
}

AOSSL::CommandLineInterpreter::CommandLineInterpreter(const std::vector<std::string>& args) {
  name = args[0];
  for (std::string elt : args) {
    process_arg(elt);
  }
}
