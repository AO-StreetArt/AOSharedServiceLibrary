# Command Line Argument Parser

The CommandLineInterpreter makes working with input parameters easier.  With it,
we get access to command line arguments in the form:

`./example name=abc`

We have access to an opt_exist method to determine if an option was entered.  We can also use get_opt to pull parameter values, and get_program_name to return the current program name executing on this instance.
    #include "include/commandline/factory_cli.h"
    #include "include/commandline/commandline_interface.h"

    int main( int argc, char** argv )
    {
    CommandLineInterpreterFactory cli_factory;
    CommandLineInterface *cli = cli_factory.get_command_line_interface( argc, argv );
    std::cout << cli->get_program_name() << std::endl;
    if ( cli->opt_exist("name") ) {
      std::cout << cli->get_opt("name") << std::endl;
    }

    return 0;
    }
