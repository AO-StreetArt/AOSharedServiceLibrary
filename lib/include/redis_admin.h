#include "hiredis.h"
#include "factory/redis_interface.h"
#include <string.h>
#include <string>

#ifndef REDIS_ADMIN
#define REDIS_ADMIN

class RedisAdmin : public RedisInterface
{
redisContext *c;
redisReply *reply;
void init(std::string hostname, int port, int timeout_seconds, int timeout_microseconds);
public:
  //TO-DO: Constructor accepting list of connections to Sentinels for HA setups

  //Constructors for single Redis Connections
  RedisAdmin(std::string hostname, int port);
  RedisAdmin(std::string hostname, int port, int timeout_seconds, int timeout_microseconds);

  //Destructor
  ~RedisAdmin();

  //! Load a value from Redis
  std::string load ( std::string key );

  //! Save a value to Redis
  bool save ( std::string key, std::string msg );

  //! Does a key exist in Redis?
  bool exists ( std::string key );

  //! Delete a value from Redis
  bool del ( std::string key );

  //! Expire a value in Redis after a specified number of seconds
  bool expire ( std::string key, unsigned int second);
};

#endif
