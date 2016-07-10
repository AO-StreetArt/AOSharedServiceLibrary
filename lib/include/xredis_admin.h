//An admin using the xRedis C++ Drivers for Redis

#include "xredis/xRedisClient.h"
#include <string.h>
#include <string>
#include "logging.h"

#ifndef REDIS_ADMIN
#define REDIS_ADMIN

class xRedisAdmin
{
xRedisClient xRed;
public:
	xRedisAdmin(RedisNode conn_list[], int conn_list_size);
	~xRedisAdmin();
	std::string load ( const char * key );
	bool save ( const char * key, std::string msg );
	bool exists ( const char * key );
	bool del ( const char * key );
	bool expire ( const char * key, unsigned int second);
};

#endif
