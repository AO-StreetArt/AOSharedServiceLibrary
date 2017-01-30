# Redis Administrator

Redis is a key-value store that acts as an application cache

The Redis Admin allows for quick Redis access, and exposes the below methods to do so:

* std::string load ( const char * key )
* bool save ( const char * key, std::string msg )
* bool exists ( const char * key )
* bool del ( const char * key )
* bool expire ( const char * key, unsigned int second)

    #include "aossl/redis/factory_redis.h"
    #include "aossl/redis/redis_interface.h"

    RedisComponentFactory redis_factory;
    RedisInterface *redis = redis_factory.get_redis_interface( "127.0.0.1", 6379 );

    //Now, we can access our basic Redis Operations
    bool bRet = redis->save("Test", "123");
    bool eRet = redis->exists("Test");
    std::string strValue = redis->load("Test");
    redis->del("Test");

The Redis Administrator suports a number of other operations, all of which are covered in [Redis' documentation] (http://redis.io/commands).  A full list of supported operations can be found in this libraries HTML documentation, found in the html folder of the docs on this repository.  You are free to download it and view it in a web browser.
