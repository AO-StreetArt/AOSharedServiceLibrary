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

// Constructor
CommandLineInterpreter::CommandLineInterpreter(int argc, char* argv[]) {
  name = argv[0];
  for (int i = 1; i < argc; i++) {
    // Find the command line parameter string and look for an equals sign
    std::string full_line(argv[i]);
    std::size_t pos = full_line.find("=");

    // We have a key-value pair
    if (pos != std::string::npos) {
      std::string key = full_line.substr(0, pos);
      std::string value = full_line.substr(pos+1);
      opts.emplace(key, value);
    } else {
      // We have an opt flag, and these can be driven by opt_exist directly
      std::string key = full_line;
      std::string value = "N/A";
      opts.emplace(full_line, value);
    }
  }
}

// Does a key exist?
bool CommandLineInterpreter::opt_exist(std::string key) {
  auto search = opts.find(key);
  if (search != opts.end()) {
    return true;
  } else {
    return false;
  }
}
