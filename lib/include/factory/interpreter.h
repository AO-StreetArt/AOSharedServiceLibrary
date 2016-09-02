#include <string>

#ifndef INTERPRETER_INTERFACE
#define INTERPRETER_INTERFACE

//! Interpreter Interface

//! The interpreter generates a list of options that are exposed based on some
//! input, whether it is via command line, properties file, or other methods.
class Interpreter
{
public:

  virtual ~Interpreter() {}

  //! Does a key exist?
  virtual bool opt_exist( std::string key ) = 0;

  //! Get an option by key
  virtual std::string get_opt( std::string key ) = 0;
  
};

#endif
