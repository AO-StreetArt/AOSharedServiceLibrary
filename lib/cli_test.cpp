#include "include/factory_cli.h"
#include "include/factory/commandline_interface.h"
#include <iostream>
#include <assert.h>

int main( int argc, char** argv )
{

CommandLineInterpreterFactory cli_factory;
CommandLineInterface *cli = cli_factory.get_command_line_interface( argc, argv );
std::cout << cli->get_program_name() << std::endl;
assert ( cli->get_program_name() == "./cli_test" );
assert ( cli->opt_exist("name") );
if ( cli->opt_exist("name") ) {
  std::cout << cli->get_opt("name") << std::endl;
  assert ( cli->get_opt("name") == "test" );
}

delete cli;
return 0;
}
