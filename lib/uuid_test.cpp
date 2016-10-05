#include "include/uuid_admin.h"
#include <iostream>
#include <assert.h>
#include <string>
#include <string.h>

int main()
{
  //Initialize the UUID Admin
  uuidAdmin uuid;

  //Generate a UUID
  std::string id_str;
  try {
    id_str = ua->generate();
  }
  catch (UuidSecurityException& ue) {
    //Continue and print the security error
    uuid_logging->error("UUID Security Exception");
    uuid_logging->error(ue.what());
  }
  assert ( !id_str.empty() );
  std::cout << id_str << std::endl;

}
