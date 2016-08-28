#include <string>
#include "interpreter.h"

#ifndef PROPERTIES_READER_INTERFACE
#define PROPERTIES_READER_INTERFACE

//! PropertiesReaderInterface

//! Here we create a new interpreter by passing in a single argument, the address
//! of a properties file.  This file is opened and read, with properties in the form:
//! property_name=property_value
class PropertiesReaderInterface: public Interpreter
{
public:

  virtual ~PropertiesReaderInterface() {}

  //! Does a key exist?
  virtual bool opt_exist( std::string key ) = 0;

  //! Get an option by key
  virtual std::string get_opt( std::string key ) = 0;

};

#endif
