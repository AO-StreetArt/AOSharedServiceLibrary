/*
MIT License Block

Copyright (c) 2016 Alex Barry

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

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
