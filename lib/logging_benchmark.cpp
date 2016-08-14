#include <hayai/hayai.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdlib>

#include "include/factory/logging_interface.h"
#include "include/factory.h"

//----------------------------------------------------------------------------//
//------------------------------Benchmarks------------------------------------//
//----------------------------------------------------------------------------//

BENCHMARK(LOGGING, Debug, 10, 100)
{

logging->debug("Testing");

}

BENCHMARK(LOGGING, Error, 10, 100)
{

logging->error("Testing");

}

BENCHMARK(LOGGING, Info, 10, 100)
{

logging->info("Testing");

}

//----------------------------------------------------------------------------//
//------------------------------Main Method-----------------------------------//
//----------------------------------------------------------------------------//

int main()
{

ServiceComponentFactory factory;

//Read the Logging Configuration File
std::string initFileName = "src/test/log4cpp_test.properties";
logging = factory.get_logging_interface( initFileName );
//logging = new Logger(initFileName);

//Set up internal variables
logging->info("Internal Logging Intialized");

//------------------------------Run Tests-------------------------------------//
//----------------------------------------------------------------------------//

hayai::ConsoleOutputter consoleOutputter;

hayai::Benchmarker::AddOutputter(consoleOutputter);
hayai::Benchmarker::RunAllTests();

//-------------------------Post-Test Teardown---------------------------------//
//----------------------------------------------------------------------------//

delete logging;

return 0;

}
