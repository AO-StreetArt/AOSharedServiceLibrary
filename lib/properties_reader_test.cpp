#include "include/factory_props.h"
#include "include/factory/properties_reader_interface.h"
#include <iostream>
#include <assert.h>
#include <vector>

int main( int argc, char** argv )
{

PropertyReaderFactory prop_factory;
PropertiesReaderInterface *reader = prop_factory.get_properties_reader_interface( "test/test.properties" );
if ( reader->opt_exist("DB_Password") ) {
  assert(reader->get_opt("DB_Password") == "default");
}
else
{
  assert(false);
}

if ( reader->list_exist("RedisConnectionString") ) {
  std::vector<std::string> list = reader->get_list("RedisConnectionString");
  assert(list.size() == 2);
}
else
{
  assert(false);
}

return 0;
}
