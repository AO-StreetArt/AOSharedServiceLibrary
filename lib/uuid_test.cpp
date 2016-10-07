#include "include/factory/uuid_interface.h"
#include "include/factory_uuid.h"
#include <iostream>
#include <assert.h>
#include <string>
#include <string.h>

int main()
{
  uuidComponentFactory id_factory;
  //Initialize the UUID Admin
  uuidInterface *uuid = id_factory.get_uuid_interface();

  //Generate a UUID
  UuidContainer id_container = uuid->generate();
  std::string id_str = id_container.id;
  assert ( !(id_str.empty()) );
  std::cout << id_str << std::endl;
  std::cout << id_container.err << std::endl;
}
