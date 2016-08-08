//An admin using the xRedis C++ Drivers for Redis

#include "xredis/xRedisClient.h"
#include <string.h>
#include <string>
#include "logging.h"

#ifndef REDIS_ADMIN
#define REDIS_ADMIN

//! The XRedis Admin

//! The XRedis Admin is responsible for all interactions with the Redis Key-Value
//! Store.  This is capable of connecting to single Redis instances or Clusters
class xRedisAdmin
{
xRedisClient xRed;
public:

	//! Construct a new XRedis Admin with a given list of Redis Connections
	xRedisAdmin(RedisNode conn_list[], int conn_list_size);

	//! Delete the XRedis Admin
	~xRedisAdmin();

	//! Load a value from Redis
	std::string load ( const char * key );

	//! Save a value to Redis
	bool save ( const char * key, std::string msg );

	//! Does a key exist in Redis?
	bool exists ( const char * key );

	//! Delete a value from Redis
	bool del ( const char * key );

	//! Expire a value in Redis after a specified number of seconds
	bool expire ( const char * key, unsigned int second);
};

#endif
