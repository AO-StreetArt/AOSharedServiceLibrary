#include <hayai/hayai.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdlib>

#include "include/factory/logging_interface.h"
#include "include/factory/uuid_interface.h"
#include "include/factory_logging.h"
#include "include/factory_uuid.h"

uuidInterface *uuid;

//----------------------------------------------------------------------------//
//------------------------------Benchmarks------------------------------------//
//----------------------------------------------------------------------------//

BENCHMARK(UUID, Generate, 10, 100)
{

std::string uuid_str = uuid->generate();

}

//----------------------------------------------------------------------------//
//------------------------------Main Method-----------------------------------//
//----------------------------------------------------------------------------//

int main()
{

  uuidComponentFactory uuid_factory;
  LoggingComponentFactory logging_factory;

//Read the Logging Configuration File
std::string initFileName = "test/log4cpp_test.properties";
logging = logging_factory.get_logging_interface( initFileName );
//logging = new Logger(initFileName);

//Set up internal variables
logging->info("Internal Logging Intialized");

//Set up UUID Generator
uuid = uuid_factory.get_uuid_interface();
//uuid = new uuidAdmin;
logging->info("UUID Generator Created");

//------------------------------Run Tests-------------------------------------//
//----------------------------------------------------------------------------//

hayai::ConsoleOutputter consoleOutputter;

hayai::Benchmarker::AddOutputter(consoleOutputter);
hayai::Benchmarker::RunAllTests();

//-------------------------Post-Test Teardown---------------------------------//
//----------------------------------------------------------------------------//

delete uuid;
delete uuid_logging;
delete logging;

return 0;

}
