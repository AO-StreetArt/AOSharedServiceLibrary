#include "hiredis/hiredis.h"
#include "factory/redis_interface.h"
#include <string.h>
#include <string>
#include <vector>

#ifndef REDIS_ADMIN
#define REDIS_ADMIN

class RedisAdmin : public RedisInterface
{
redisContext *c = NULL;
redisReply *reply = NULL;
void init(std::string hostname, int port, int timeout_seconds, int timeout_microseconds);
bool process_std_string_reply(redisReply *reply);
bool process_std_int_reply(redisReply *reply);
int return_int_reply(redisReply *reply);
std::string return_string_reply(redisReply *reply);
public:
  //TO-DO: Constructor accepting list of connections to Sentinels for HA setups

  //Constructors for single Redis Connections
  RedisAdmin(std::string hostname, int port);
  RedisAdmin(std::string hostname, int port, int timeout_seconds, int timeout_microseconds);

  //Constructors for lists of Redis Connections (Sentinels)
  RedisAdmin(RedisConnChain connection_list);

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

  bool persist ( std::string key );

  int incr ( std::string key );
  int incr ( std::string key, int incr_amt );
  int decr ( std::string key );
  int decr ( std::string key, int decr_amt );

  bool setex ( std::string key, std::string val, unsigned int second);

  bool append ( std::string key, std::string val );

  int len ( std::string key );

  int lpush ( std::string key, std::string val );
  int rpush ( std::string key, std::string val );

  //! Pop a value from a Redis list on the given key
	std::string lpop ( std::string key );

  //! Pop a value from a Redis list on the given key
  std::string rpop ( std::string key );

  //! Set the value stored in the list at key and the index at index
  bool lset ( std::string key, std::string val, int index);

  //! Insert a value into the list at key and before/after the pivot value
  int linsert ( std::string key, std::string val, std::string pivot, bool before_pivot);

  //! Get the value stored in the list at key and the index at index
  std::string lindex ( std::string key, int index);

  //! Get the length of a list stored in Redis
  int llen ( std::string key );

  //! Trim a list to the specified start and end index
  bool ltrim ( std::string key, int start_index, int end_index);

  bool mset ( std::vector<RedisKvPair> save_sets );

  std::vector<std::string> mget ( std::vector<std::string> keys );

  bool msetnx ( std::vector<RedisKvPair> save_sets );

  bool setnx ( std::string key, std::string msg );
};

#endif
