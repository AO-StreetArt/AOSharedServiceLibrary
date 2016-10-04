//Tests for Redis Admin

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <stdlib.h>
#include <unistd.h>

#include "include/factory/redis_interface.h"

#include "include/redis_admin.h"

#include <assert.h>

//Main Method

std::vector<RedisConnChain> RedisConnectionList;

RedisAdmin *xRedis;

int main()
{

//Set up Redis Connection
xRedis = new RedisAdmin ("127.0.0.1", 6379);

//---------------------------Core Components----------------------------------//

//save
assert( xRedis->save("Test", "123") );

//exists
assert( xRedis->exists("Test") );

//load
std::string strValue = xRedis->load("Test");
assert(strValue == "123");
std::cout << strValue << std::endl;

//Delete
assert( xRedis->del("Test") );
assert( !(xRedis->exists("Test")) );

//Expire & Persist
assert( xRedis->save("1", "123") );
assert( xRedis->save("2", "456") );
assert( xRedis->save("3", "789") );
assert( xRedis->expire("1", 1) );
assert( xRedis->expire("2", 10) );
assert( xRedis->expire("3", 5) );
assert( xRedis->exists("1") );
assert( xRedis->exists("2") );
assert( xRedis->exists("3") );

sleep(2000);
assert( !(xRedis->exists("1")) );
assert( xRedis->exists("2") );
assert( xRedis->exists("3") );

assert( xRedis->persist("3") );

sleep(20000);
assert( !(xRedis->exists("1")) );
assert( !(xRedis->exists("2")) );
assert( xRedis->exists("3") );

//Setex
assert( xRedis->setex("1", "123", 1) );
assert( xRedis->setex("2", "456", 10) );
assert( xRedis->exists("1") );
assert( xRedis->exists("2") );

sleep(2000);
assert( !(xRedis->exists("1")) );
assert( xRedis->exists("2") );

sleep(20000);
assert( !(xRedis->exists("1")) );
assert( !(xRedis->exists("2")) );

//Length
assert( xRedis->len("3") == 3 );

//append
assert( xRedis->append("3", "1011") );
assert( xRedis->len("3") == 7 );

//---------------------------Counter Methods----------------------------------//

//Increase
assert( xRedis->incr("counter") == 1);
assert( xRedis->incr("counter") == 2);

//Decrease
assert( xRedis->decr("counter") == 1);
assert( xRedis->decr("counter") == 0);

delete xRedis;
return 0;
}
