#include <hayai/hayai.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <uuid/uuid.h>

#include "include/factory/logging_interface.h"
#include "include/factory/redis_interface.h"
#include "include/factory.h"

RedisInterface *xRedis;
std::vector<std::string> uuid_list;
int savecounter = 0;
int getcounter = 0;
int delcounter = 0;
int existcounter = 0;

//----------------------------------------------------------------------------//
//------------------------------Benchmarks------------------------------------//
//----------------------------------------------------------------------------//

BENCHMARK(Redis, Save, 10, 100)
{

std::string uuid_str = uuid_list[savecounter];

//save
bool bRet = xRedis->save( uuid_str.c_str(), "123");
if (!bRet) {
logging->error("Error putting object to Redis Smart Update Buffer");
}

savecounter=savecounter+1;

}

BENCHMARK(Redis, ExistsTrue, 10, 100)
{

std::string uuid_str = uuid_list[existcounter];

//exists
bool eRet = xRedis->exists( uuid_str.c_str() );

existcounter=existcounter+1;

}

BENCHMARK(Redis, ExistsFalse, 10, 100)
{

std::string uuid_str = "TEST";

//exists
bool eRet = xRedis->exists( uuid_str.c_str() );

}

BENCHMARK(Redis, Load, 10, 100)
{

std::string uuid_str = uuid_list[getcounter];

//load
std::string strValue = xRedis->load( uuid_str.c_str() );

getcounter=getcounter+1;

}

BENCHMARK(Redis, Delete, 10, 100)
{

std::string uuid_str = uuid_list[delcounter];

//Delete
xRedis->del( uuid_str.c_str() );

delcounter=delcounter+1;

}

//----------------------------------------------------------------------------//
//------------------------------Main Method-----------------------------------//
//----------------------------------------------------------------------------//

int main()
{

//Application Setup
std::vector<RedisConnChain> RedisConnectionList;

//Read the Redis Configuration File
//Open the file
std::string line;
std::ifstream file ("test/redis.properties");

if (file.is_open()) {
  while (getline (file, line) ) {
    //Read a line from the property file

    //Figure out if we have a blank or comment line
    bool keep_going = true;
    if (line.length() > 0) {
      if (line[0] == '/' && line[1] == '/') {
        keep_going=false;
      }
    }
    else {
      keep_going=false;
    }

    if (keep_going==true) {
      int eq_pos = line.find("=", 0);
      std::string var_name = line.substr(0, eq_pos);
      std::string var_value = line.substr(eq_pos+1, line.length() - eq_pos);
      if (var_name=="RedisConnectionString") {
        //Read a string in the format 127.0.0.1--7000----2--5--0
        RedisConnChain chain;

        //Retrieve the first value
        int spacer_position = var_value.find("--", 0);
        std::string str1 = var_value.substr(0, spacer_position);
        chain.ip = str1;

        //Retrieve the second value
        std::string new_value = var_value.substr(spacer_position+2, var_value.length() - 1);
        spacer_position = new_value.find("--", 0);
        str1 = new_value.substr(0, spacer_position);
        chain.port = std::stoi(str1);

        //Retrieve the third value
        new_value = new_value.substr(spacer_position+2, new_value.length() - 1);
        spacer_position = new_value.find("--", 0);
        str1 = new_value.substr(0, spacer_position);
        chain.password = str1;

        //Retrieve the fourth value
        new_value = new_value.substr(spacer_position+2, new_value.length() - 1);
        spacer_position = new_value.find("--", 0);
        str1 = new_value.substr(0, spacer_position);
        chain.pool_size = std::stoi(str1);

        //Retrieve the fifth value
        new_value = new_value.substr(spacer_position+2, new_value.length() - 1);
          spacer_position = new_value.find("--", 0);
          str1 = new_value.substr(0, spacer_position);
        chain.timeout = std::stoi(str1);

        //Retrieve the final value
        new_value = new_value.substr(spacer_position+2, new_value.length() - 1);
        spacer_position = new_value.find("--", 0);
        str1 = new_value.substr(0, spacer_position);
        chain.role = std::stoi(str1);

        RedisConnectionList.push_back(chain);
      }
    }
  }
  file.close();
}

//Set up the Service Factory
ServiceComponentFactory factory;

//Read the Logging Configuration File
std::string initFileName = "test/log4cpp_test.properties";
logging = factory.get_logging_interface( initFileName );

//Generate the UUID's for the benchmarks
int i=0;
for (i=0; i< 1001; i++) {
  //Generate a new key for the object
  std::string uuid_str = std::to_string(i);
  uuid_list.push_back(uuid_str);
}

//Set up Redis Connection
xRedis = factory.get_redis_cluster_interface(RedisConnectionList);
logging->info("Connected to Redis");

//------------------------------Run Tests-------------------------------------//
//----------------------------------------------------------------------------//

hayai::ConsoleOutputter consoleOutputter;

hayai::Benchmarker::AddOutputter(consoleOutputter);
hayai::Benchmarker::RunAllTests();

//-------------------------Post-Test Teardown---------------------------------//
//----------------------------------------------------------------------------//

delete xRedis;
delete redis_logging;
delete logging;

return 0;

}
