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

std::unordered_map<std::string, std::string> map = reader.get_opts();

std::vector<std::string> keys;
keys.reserve(map.size());
std::vector<std::string> vals;
vals.reserve(map.size());

for(auto kv : map) {
    keys.push_back(kv.first);
    vals.push_back(kv.second);
}

for (size_t i = 0; i < keys.size(); i++)
{
  std::cout << keys[i] << ":" << vals[i] << std::endl;
}

return 0;
}
