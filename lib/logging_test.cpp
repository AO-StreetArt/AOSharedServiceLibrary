#include "include/logging.h"
#include "include/factory/logging_interface.h"
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

  LoggingCategoryInterface *sub_category = logging->get_category("tertiary");
  LoggingCategoryInterface *third_category = logging->get_category("secondary.sub1");
  LoggingCategoryInterface *bad_category = logging->get_category("incorrect_name");

  logging->debug("Testing");
  logging->error("Testing");
  std::string test = "123";
  test_func(test);

  sub_category->error("Testing");
  third_category->error("Testing");
  bad_category->error("Testing");

  delete sub_category;
  delete third_category;
  delete bad_category;
  delete logging;

}
