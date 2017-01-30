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

#include <unordered_map>
#include <string>
#include <iterator>

#include "commandline_interface.h"

#ifndef COMMAND_LINE
#define COMMAND_LINE

class CommandLineInterpreter: public CommandLineInterface
{
  //An unordered map of options from the command line
  std::unordered_map<std::string, std::string> opts;
  std::string name;
public:

  //! Create a new Command Line Interpreter

  //! Here we create a new interpreter by passing in the two arguments from the main method,
  //! int argc & char* argv[].  This parses arguments passed in the form:
  //! -arg_key=arg_val
  CommandLineInterpreter(int argc, char* argv[]);

  ~CommandLineInterpreter() {}

  //! Does a key exist?
  bool opt_exist( std::string key );

  //! Get an option by key
  std::string get_opt( std::string key ) {return opts[key];}

  //! Get the program name

  //! Get the name of the executable currently being
  //! run.
  std::string get_program_name() {return name;}
};

#endif
