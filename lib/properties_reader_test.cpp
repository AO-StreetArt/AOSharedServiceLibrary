#include "include/properties_reader.h"
#include <iostream>
#include <assert.h>
#include <vector>

int main( int argc, char** argv )
{

PropertiesReader reader ( "test/test.properties" );
if ( reader.opt_exist("DB_Password") ) {
  assert(reader.get_opt("DB_Password") == "default");
}
else
{
  assert(false);
}

if ( reader.list_exist("RedisConnectionString") ) {
  std::vector<std::string> list = reader.get_list("RedisConnectionString");
  assert(list.size() == 2);
}
else
{
  assert(false);
}

return 0;
}
