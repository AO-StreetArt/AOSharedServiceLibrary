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

#include "include/factory_cli.h"
#include "include/commandline_interface.h"
#include <iostream>
#include <assert.h>

int main( int argc, char** argv )
{

  CommandLineInterpreterFactory cli_factory;
  CommandLineInterface *cli = cli_factory.get_command_line_interface( argc, argv );
  std::cout << cli->get_program_name() << std::endl;
  assert ( cli->get_program_name() == "./cli_test" );
  assert ( cli->opt_exist("name") );
  if ( cli->opt_exist("name") ) {
    std::cout << cli->get_opt("name") << std::endl;
    assert ( cli->get_opt("name") == "test" );
  }

  delete cli;
  return 0;
}
