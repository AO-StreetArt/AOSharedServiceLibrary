#include "include/uuid_admin.h"
#include "include/factory/logging_interface.h"
#include "include/logging.h"

int main()
{
  //Set up logging
  std::string initFileName = "test/log4cpp_test.properties";
  logging = new Logger(initFileName);

  //Initialize the UUID Admin
  uuidAdmin uuid;

  //Generate a UUID
  std::string uuid_str = uuid.generate();
  logging->debug(uuid_str);

  delete logging;

}
