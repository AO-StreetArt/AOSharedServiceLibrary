#include "include/uuid_admin.h"
#include "include/logging.h"

int main()
{
  //Set up logging
  std::string initFileName = "test/log4cpp_test.properties";
  try {
  	log4cpp::PropertyConfigurator::configure(initFileName);
  }
  catch ( log4cpp::ConfigureFailure &e ) {
  	printf("[log4cpp::ConfigureFailure] caught while reading Logging Configuration File");
  	printf(e.what());
  	exit(1);
  }

  log4cpp::Category& root = log4cpp::Category::getRoot();

  log4cpp::Category& sub1 = log4cpp::Category::getInstance(std::string("sub1"));

  log4cpp::Category& log = log4cpp::Category::getInstance(std::string("sub1.log"));

  logging = &log;

  //Initialize the UUID Admin
  uuidAdmin uuid;

  //Generate a UUID
  std::string uuid_str = uuid.generate();
  logging->debug(uuid_str);

}
