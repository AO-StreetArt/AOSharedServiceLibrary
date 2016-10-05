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
  bool generated = false;
  try {
    id_str = uuid.generate();
    generated = true;
  }
  catch (UuidSecurityException& ue) {
    //Continue and print the security error
    std::cout << "UUID Security Exception" << std::endl;
    std::cout << ue.what() << std::endl;
  }
  if (generated) {
    assert ( !id_str.empty() );
    std::cout << id_str << std::endl;
  }
}
