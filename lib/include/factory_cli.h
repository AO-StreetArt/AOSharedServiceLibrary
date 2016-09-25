#include "factory/commandline_interface.h"
#include "cli.h"

#ifndef AOSSL_FACTORY_COMMANDLINE
#define AOSSL_FACTORY_COMMANDLINE

//! The Command Line Interpreter Service Component Factory

//! The Service Component Factory tracks the CLI
//! objects exposed by the framework and passes back
//! instances of interfaces.  This allows for the publicly exposed methods
//! to be independent of the implementations.
class CommandLineInterpreterFactory
{
public:

  //! Create a new Service Component Factory
  CommandLineInterpreterFactory() {}

  //! Delete a Service Component Factory
  ~CommandLineInterpreterFactory() {}

  //! Get the Command Line Interface instance
  CommandLineInterface* get_command_line_interface( int argc, char* argv[] ) {return new CommandLineInterpreter( argc, argv );}
};

#endif
