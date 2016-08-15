#include <iostream>
#include <string>
#include <exception>
#include <signal.h>

#include "aossl/factory.h"

#include "aossl/factory/commandline_interface.h"
#include "aossl/factory/logging_interface.h"

//Shutdown the application
void shutdown()
{
  //Delete objects off the heap
  delete cli;
  delete logging;
}

//Catch a Signal (for example, keyboard interrupt)
void my_signal_handler(int s){
   logging->error("Caught signal");
   std::string signal_type = std::to_string(s);
   logging->error(signal_type);
   shutdown();
   exit(1);
}

int main( int argc, char** argv )
{

//Set up a handler for any signal events so that we always shutdown gracefully
struct sigaction sigIntHandler;
sigIntHandler.sa_handler = my_signal_handler;
sigemptyset(&sigIntHandler.sa_mask);
sigIntHandler.sa_flags = 0;

sigaction(SIGINT, &sigIntHandler, NULL);

// Set up a Service Component Factory, where we get our application components
ServiceComponentFactory factory;

//Retrieve a new command line interface
cli = factory.get_command_line_interface(argc, argv);

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
logging = factory.get_logging_interface( initFileName );

//Say Hello!
logging->error("Hello world!");

//Finally, we cleanup the app

shutdown();

return 0;
}
