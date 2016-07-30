#include "include/cli.h"
#include <iostream>

int main( int argc, char** argv )
{

CommandLineInterpreter cli ( argc, argv );
std::cout << cli.get_program_name() << std::endl;
if ( cli.opt_exist("name") ) {
  std::cout << cli.get_opt("name") << std::endl;
}

return 0;
}
