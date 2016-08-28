#include "include/properties_reader.h"
#include <iostream>
#include <assert.h>

int main( int argc, char** argv )
{

PropertiesReader reader ( "test/test.properties" );
if ( reader.opt_exist("DB_Password") ) {
  assert(reader.get_opt("DB_Password") == "default");
}

return 0;
}
