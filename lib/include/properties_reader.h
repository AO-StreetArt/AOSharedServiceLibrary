#include <unordered_map>
#include <string>
#include <iterator>

#include <sys/stat.h>
#include <sstream>
#include <fstream>

#include "factory/properties_reader_interface.h"

#ifndef PROPS_READER
#define PROPS_READER

class PropertiesReader: public PropertiesReaderInterface
{
//An unordered map of options from the command line
std::unordered_map<std::string, std::string> opts;
std::unordered_map<std::string, std::vector<std::string>> opt_lists;
std::string name;
public:

  //! Create a new Command Line Interpreter

  //! Here we create a new interpreter by passing in a single argument, the address
  //! of a properties file.
  PropertiesReader(std::string file_path);

  ~PropertiesReader() {}

  //! Does a key exist?
  bool opt_exist( std::string key );

  //! Get an option by key
  std::string get_opt( std::string key ) {return opts[key];}

  //! Check if a parameter list exists
  bool list_exist( std::string key );

  //! Get a parameter list
  std::vector<std::string> get_list( std::string key ) {return opt_lists[key];}

  //Get the option list
  std::unordered_map<std::string, std::string> get_opts() {return opts;}

};

#endif
