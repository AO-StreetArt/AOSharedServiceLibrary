#include "include/cli.h"
#include <iostream>

int main( int argc, char** argv )
{

CommandLineInterpreter cli ( argc, argv );
std::cout << cli.get_program_name() << std::endl;
assert ( cli.get_program_name() == "cli_test" );
assert ( cli.opt_exist("name") );
if ( cli.opt_exist("name") ) {
  std::cout << cli.get_opt("name") << std::endl;
  assert ( cli.get_opt("name") == "test" );
}

return 0;
}
