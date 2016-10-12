#include <iostream>
#include <string>
#include <exception>
#include <signal.h>

#include "aossl/factory_cli.h"
#include "aossl/factory_logging.h"

#include "aossl/factory/commandline_interface.h"
#include "aossl/factory/logging_interface.h"

CommandLineInterface *cli = NULL;

//Shutdown the application
void shutdown()
{
  //Delete objects off the heap
  if (cli) {
    delete cli;
  }
  delete logging;
}

int main( int argc, char** argv )
{

// Set up a Service Component Factory, where we get our application components
CommandLineInterpreterFactory cli_factory;
LoggingComponentFactory logging_factory;

//Retrieve a new command line interface
cli = cli_factory.get_command_line_interface(argc, argv);

std::string initFileName;

//See if we have a command line setting for the log file
if ( cli->opt_exist("-log-conf") ) {
  initFileName = cli->get_opt("-log-conf");
}
else
{
  initFileName = "log4cpp_test.properties";
}

//! Get a Logging Interface instance, and pass to the global logging instance
logging = logging_factory.get_logging_interface( initFileName );

//Say Hello!
logging->error("Hello world!");

//Finally, we cleanup the app

shutdown();

return 0;
}
