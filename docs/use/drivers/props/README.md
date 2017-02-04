# Property File Reader

The property file reader accepts properties file names and interprets them.  It accepts properties in the form `property_name=property_value`.  This also accepts lists in the form

    -list_name-list_value
    -list_name-list_value2

    #include "aossl/properties/factory_props.h"
    #include "aossl/properties/properties_reader_interface.h"
    #include <string>
    #include <vector>

    PropertyReaderFactory prop_factory;
    PropertiesReaderInterface *reader = prop_factory.get_properties_reader_interface( "test.properties" );

We can access properties via the get_opt method, and lists via the get_list method.  We also have opt_exist and list_exist methods so we can determine if something has been detected in the configuration file.

    if ( reader->opt_exist("DB_Password") ) {
      std::string db_pswd = reader->get_opt("DB_Password");
    }

    if ( reader->list_exist("RedisConnectionString") ) {
      std::vector<std::string> list = reader->get_list("RedisConnectionString");
    }
