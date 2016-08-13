#include <unordered_map>
#include <string>
#include <iterator>

#include "factory/commandline_interface.h"

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
