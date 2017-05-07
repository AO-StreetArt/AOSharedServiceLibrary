//tests for the configuration manager
//src/test/test.properties

#include <assert.h>

#include "aossl/logging/include/logging_interface.h"
#include "aossl/logging/include/factory_logging.h"

#include "app_log.h"

int main( int argc, char** argv )
{

  LoggingComponentFactory *logging_factory = new LoggingComponentFactory;

  //-------------------------------Logging--------------------------------------//
  //----------------------------------------------------------------------------//

  std::string initFileName = "tests/log4cpp.properties";
  logging = logging_factory->get_logging_interface(initFileName);

  start_logging_submodules();

  logging->debug("Debug Test");
  logging->info("Info Test");
  logging->error("Error Test");

  shutdown_logging_submodules();

  delete logging;
  delete logging_factory;
  return 0;
}
