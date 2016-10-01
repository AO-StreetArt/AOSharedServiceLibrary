//Tests for Redis Admin

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>

#include "include/factory/redis_interface.h"

#include "include/redis_admin.h"

#include <assert.h>

//Main Method

std::vector<RedisConnChain> RedisConnectionList;

xRedisAdmin *xRedis;

int main()
{

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

//Set up the Redis Admin
//Set up our Redis Connection List
int conn_list_size = RedisConnectionList.size();
RedisNode RedisList1[conn_list_size];
for (int y = 0; y < conn_list_size; ++y)
{
  //Pull the values from RedisConnectionList
  RedisNode redis_n;
  redis_n.dbindex = y;
  RedisConnChain redis_chain = RedisConnectionList[y];
  redis_n.host = redis_chain.ip.c_str();
  redis_n.port = redis_chain.port;
  redis_n.passwd = redis_chain.password.c_str();
  redis_n.poolsize = redis_chain.pool_size;
  redis_n.timeout = redis_chain.timeout;
  redis_n.role = redis_chain.role;
  std::cout << "Line added to Redis Configuration List with IP:" << std::endl;
  std::cout << redis_n.host << std::endl;

  RedisList1[y] = redis_n;
}
std::cout << "Redis Connection List Built" << std::endl;

//Set up Redis Connection
xRedis = new RedisAdmin ("127.0.0.1", 6379);
std::cout << "Connected to Redis" << std::endl;

//save
bool bRet = xRedis->save("Test", "123");
if (!bRet) {
std::cout << "Error putting object to Redis Smart Update Buffer" << std::endl;
assert(bRet);
}

//exists
bool eRet = xRedis->exists("Test");
if (!eRet) {
std::cout << "Test not found in buffer" << std::endl;
assert(eRet);
}

//load
std::string strValue = xRedis->load("Test");
assert(strValue == "123");
std::cout << strValue << std::endl;

//Delete
xRedis->del("Test");

delete xRedis;
return 0;
}
