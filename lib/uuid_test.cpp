#include "include/uuid_admin.h"
#include "include/factory/uuid_interface.h"
#include <iostream>
#include <assert.h>
#include <string>
#include <string.h>

int main()
{
  //Initialize the UUID Admin
  uuidAdmin uuid;

  //Generate a UUID
  UuidContainer id_container = uuid.generate();
  std::string id_str = id_container.id;
  assert ( !(id_str.empty()) );
  std::cout << id_str << std::endl;
  std::cout << id_container.err << std::endl;
}
