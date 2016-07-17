#include "include/cli.h" 

//Constructor
CommandLineInterpreter::CommandLineInterpreter(int argc, char* argv[]) 
{
  name = argv[0];
  for (int i = 1; i < argc; i++)
  {
    std::string full_line (argv[i]);
    std::size_t pos = full_line.find("=");
    std::string key = full_line.substr ( 0, pos );
    std::string value = full_line.substr( pos+1 );
    opts.emplace( key, value );
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

