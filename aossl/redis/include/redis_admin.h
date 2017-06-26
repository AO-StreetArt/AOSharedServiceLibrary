/*
MIT License Block

Copyright (c) 2016 Alex Barry

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

//TO-DO: When an operation fails, it should automatically detect whether there are
//Sentinels available and, if so, try to failover to another Redis Node.

#ifndef REDIS_ADMIN
#define REDIS_ADMIN

#include "hiredis/hiredis.h"
#include "redis_interface.h"
#include <string.h>
#include <string>
#include <vector>
#include <exception>
#include <mutex>
#include <iostream>

//A struct containing the objects needed to run a query
struct RedisSession {
  redisContext *connection = NULL;
  int index = -1;
};

//A Connection pool to ensure thread safety
class RedisConnectionPool {
  //A pool of neo4j connections
  std::vector<RedisSession> connections;
  //Array of ints (0/1) which determine which connections are open vs closed
  int *slots;
  //Internal integers
  int connection_limit = 1;
  int start_connections = 1;
  int current_connection = -1;
  int current_max_connection = 1;
  int connection_creation_batch = 1;
  int timeout_seconds = 5;
  int timeout_microseconds = 0;
  std::string connection_string;
  std::string password;
  std::string err_msg;
  int port;
  std::mutex get_conn_mutex;
  void init_slots();
  void init_connections(const char * conn_str, const char * passwd, int con_port, int timeout_secs, int timeout_microsecs);
public:
  RedisConnectionPool(int size, const char * conn_str, const char * passwd, int con_port) {connection_limit=size;init_slots();init_connections(conn_str, passwd, con_port, timeout_seconds, timeout_microseconds);}
  RedisConnectionPool(int size, const char * conn_str, const char * passwd, int con_port, int timeout_secs, int timeout_microsecs) {connection_limit=size;init_slots();init_connections(conn_str, passwd, con_port, timeout_secs, timeout_microsecs);}
  RedisConnectionPool(int size, const char * conn_str, const char * passwd, int con_port, int timeout_secs, int timeout_microsecs, int start_conns) {init_slots();start_connections=start_conns;connection_limit=size;init_connections(conn_str, passwd, con_port, timeout_secs, timeout_microsecs);}
  RedisConnectionPool(int size, const char * conn_str, const char * passwd, int con_port, int timeout_secs, int timeout_microsecs, int start_conns, int batch_size) {init_slots();start_connections=start_conns;connection_limit=size;connection_creation_batch=batch_size;init_connections(conn_str, passwd, con_port, timeout_secs, timeout_microsecs);}
  ~RedisConnectionPool();
  RedisSession* get_connection();
  void release_connection(RedisSession *conn);
};

//The Redis Administrator that exposes key methods
class RedisAdmin : public RedisInterface
{
  std::string reply_str;
  std::string int_hostname;
  RedisConnectionPool *pool = NULL;
  void init(std::string hostname, std::string passwd, int port, int timeout_seconds, int timeout_microseconds, int pool_size);
  bool process_std_string_reply(redisReply *reply);
  bool process_std_int_reply(redisReply *reply);
  int return_int_reply(redisReply *reply);
  void return_string_reply(redisReply *reply);
public:
  //TO-DO: Constructor accepting list of connections to Sentinels for HA setups

  //Constructors for single Redis Connections
  RedisAdmin(std::string hostname, int port);
  RedisAdmin(std::string hostname, int port, int pool_size);
  RedisAdmin(std::string hostname, int port, int timeout_seconds, int timeout_microseconds);
  RedisAdmin(std::string hostname, int port, int timeout_seconds, int timeout_microseconds, int pool_size);

  //Constructors for lists of Redis Connections (Sentinels)
  RedisAdmin(RedisConnChain connection_list);

  //Constructors for lists of Redis Connections (Sentinels)
  RedisAdmin(RedisConnChain connection_list, int pool_size);

  //Destructor
  ~RedisAdmin() {if (pool) {delete pool;}}

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

  int append ( std::string key, std::string val );

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
