/*
MIT License Block

Copyright (c) 2016 Alex Barry

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "include/factory_logging.h"
#include "include/factory/logging_interface.h"
#include <string>
#include <string.h>

void test_func(std::string msg)
{
  logging->error(msg);
}

int main()
{
  LoggingComponentFactory log_factory;
  std::string initFileName = "test/logging_test.properties";
  logging = log_factory.get_logging_interface(initFileName);

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
