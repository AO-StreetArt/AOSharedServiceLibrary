#include "include/cli.h"

//Constructor
CommandLineInterpreter::CommandLineInterpreter(int argc, char* argv[]) {
  name = argv[0];
  for (int i = 1; i < argc; i++)
{
  //Find the command line parameter string and look for an equals sign
  std::string full_line (argv[i]);
  std::size_t pos = full_line.find("=");

  //We have a key-value pair
  if (pos != std::string::npos)
  {
  std::string key = full_line.substr ( 0, pos );
  std::string value = full_line.substr( pos+1 );
  opts.emplace( key, value );
  }

  //We have an opt flag, and these can be driven by opt_exist directly
  else
  {
    std::string key = full_line;
    std::string value = "N/A";
    opts.emplace( full_line, value );
  }
}
}

  //Does a key exist?
bool CommandLineInterpreter::opt_exist( std::string key )
{
  auto search = opts.find(key);
  if (search != opts.end())
  {
    return true;
  }
  else
  {
    return false;
  }
}
