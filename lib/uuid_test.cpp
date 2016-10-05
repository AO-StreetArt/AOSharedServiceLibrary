#include "include/uuid_admin.h"
#include <iostream>
#include <assert.h>

int main()
{
  //Initialize the UUID Admin
  uuidAdmin uuid;

  //Generate a UUID
  std::string uuid_str = uuid.generate();
  assert ( !uuid_str.empty() );
  std::cout << uuid_str << std::endl;

}
