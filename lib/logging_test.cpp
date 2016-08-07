#include "include/logging.h"
#include <string>
#include <string.h>

void test_func(std::string msg)
{
  logging->error(msg);
}

int main()
{
  std::string initFileName = "test/log4cpp_test.properties";
  logging = new Logger(initFileName);

  logging->debug("Testing");
  std::string test = "123";
  test_func(test);

  logging->get_category("sub1")->info("Sub Test");
  logging->get_category("unknown")->error("Bad Test");

  

}
