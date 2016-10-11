#include <string>
#include <vector>
#include "interpreter.h"

#ifndef PROPERTIES_READER_INTERFACE
#define PROPERTIES_READER_INTERFACE

//! PropertiesReaderInterface

//! Here we create a new interpreter by passing in a single argument, the address
//! of a properties file.  This file is opened and read, with properties in the form:
//! property_name=property_value
//! This also accepts lists in the form
//! -list_name-list_value
//! -list_name-list_value2
class PropertiesReaderInterface: public Interpreter
{
public:

  virtual ~PropertiesReaderInterface() {}

  //! Does a key exist?
  virtual bool opt_exist( std::string key ) = 0;

  //! Get an option by key
  virtual std::string get_opt( std::string key ) = 0;

  //! Does a list exist within the configuration?
  virtual bool list_exist( std::string key ) = 0;

  //! Get a list that exists within the configuration
  virtual std::vector<std::string> get_list( std::string key ) = 0;

};

#endif
