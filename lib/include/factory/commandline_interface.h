#include <string>
#include "interpreter.h"

#ifndef COMMAND_LINE_INTERFACE
#define COMMAND_LINE_INTERFACE

//! CommandLineInterface

//! Here we create a new interpreter by passing in the two arguments from the main method,
//! int argc & char* argv[].  This parses arguments passed in the form:
//! -arg_key=arg_val
class CommandLineInterface: public Interpreter
{
public:

  virtual ~CommandLineInterface() {}

  //! Does a key exist?
  virtual bool opt_exist( std::string key ) = 0;

  //! Get an option by key
  virtual std::string get_opt( std::string key ) = 0;

  //! Get the program name

  //! Get the name of the executable currently being
  //! run.
  virtual std::string get_program_name() = 0;
};

#endif
