#include <string.h>
#include "factory/redis_interface.h"
#include "redis_admin.h"

#ifndef AOSSL_FACTORY_REDIS
#define AOSSL_FACTORY_REDIS

//! The Redis Service Component Factory

//! The Service Component Factory tracks the Redis
//! objects exposed by the framework and passes back
//! instances of interfaces.  This allows for the publicly exposed methods
//! to be independent of the implementations.
class RedisComponentFactory
{

public:

  //! Create a new Service Component Factory
  RedisComponentFactory() {}

  //! Delete a Service Component Factory
  ~RedisComponentFactory() {}

  inline RedisInterface* get_redis_interface(std::string hostname, int port, int timeout_seconds, int timeout_microseconds)
  {
    return new RedisAdmin( hostname, port, timeout_seconds, timeout_microseconds);
  }

  inline RedisInterface* get_redis_interface(std::string hostname, int port)
  {
    return new RedisAdmin( hostname, port);
  }

  //! Get a Redis Cluster Interface instance
  // inline RedisInterface* get_redis_cluster_interface( std::vector<RedisConnChain> RedisConnectionList )
  // {
  //   //Set up the Redis Admin
  //   //Set up our Redis Connection List
  //   int conn_list_size = RedisConnectionList.size();
  //   RedisNode RedisList1[conn_list_size];
  //   for (int y = 0; y < conn_list_size; ++y)
  //   {
  //     //Pull the values from RedisConnectionList
  //     RedisNode redis_n;
  //     redis_n.dbindex = y;
  //     RedisConnChain redis_chain = RedisConnectionList[y];
  //     redis_n.host = redis_chain.ip.c_str();
  //     redis_n.port = redis_chain.port;
  //     redis_n.passwd = redis_chain.password.c_str();
  //     redis_n.poolsize = redis_chain.pool_size;
  //     redis_n.timeout = redis_chain.timeout;
  //     redis_n.role = redis_chain.role;
  //     logging->debug("Line added to Redis Configuration List with IP:");
  //     logging->debug(redis_n.host);
  //
  //     RedisList1[y] = redis_n;
  //   }
  //   return new xRedisAdmin( RedisList1, conn_list_size );
  // }

};

#endif
