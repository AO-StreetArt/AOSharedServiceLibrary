#include "include/logging.h"
#include <string>
#include <string.h>

void test_func(std::string msg)
{
  logging->error(msg);
}

int main()
{
  std::string initFileName = "log4cpp.properties";
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

  logging->debug("Testing");
  std::string test = "123";
  test_func(test);

}
