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
#include <assert.h>
#include <iostream>
#include <vector>
#include <string>

void validate_cli(AOSSL::CommandLineInterpreter& cli) {
  // Test the command line interpreter
  std::cout << cli.get_name() << std::endl;
  assert(cli.get_name() == "./cli_test");
  assert(cli.opt_exist("name"));
  if (cli.opt_exist("name")) {
    AOSSL::StringBuffer value_buffer;
    cli.get_opt("name", value_buffer);
    std::cout << value_buffer.val << std::endl;
    assert(value_buffer.val == "test");
  }
}

int main(int argc, char** argv) {
  std::vector<std::string> vargs;
  for (int i = 0; i < argc; i++) {
    std::string full_line(argv[i]);
    vargs.push_back(full_line);
  }

  // Build a command line interpreter with an array of args
  AOSSL::CommandLineInterpreter cli(argc, argv);

  // Test the command line interpreter
  validate_cli(cli);

  // Build the command line interpreter with a vector of args
  AOSSL::CommandLineInterpreter vcli(vargs);

  // Test the command line interpreter
  validate_cli(vcli);

  return 0;
}
