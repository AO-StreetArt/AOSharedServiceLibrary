#include <unordered_map>
#include <string>
#include <iterator>

#ifndef COMMAND_LINE
#define COMMAND_LINE

class CommandLineInterpreter
{
//An unordered map of options from the command line
std::unordered_map<std::string, std::string> opts;
std::string name;
public:

  //Constructor
  CommandLineInterpreter(int argc, char* argv[]);

  //Does a key exist?
  bool opt_exist( std::string key );

  //Get an option by key
  std::string get_opt( std::string key ) {return opts[key];}

  //Get the program name
  std::string get_program_name() {return name;}
};

#endif
