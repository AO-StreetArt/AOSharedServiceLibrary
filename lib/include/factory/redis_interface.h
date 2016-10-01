//An admin using the xRedis C++ Drivers for Redis

#include <string.h>
#include <string>

#ifndef REDIS_INTERFACE
#define REDIS_INTERFACE

struct RedisConnectionException: public std::exception
{
  std::string int_msg;
  RedisConnectionException (std::string msg) {int_msg = msg;}
  RedisConnectionException (const char * msg_cstr) {std::string msg (msg_cstr);int_msg = msg;}
  RedisConnectionException () {}
  ~RedisConnectionException() throw () {}
  const char * what() const throw ()
  {
    std::string what_str = "Error Connecting to Redis: " + int_msg;
    return what_str.c_str();
  }
};


struct RedisOperationException: public std::exception
{
  std::string int_msg;
  RedisOperationException (std::string msg) {int_msg = msg;}
  RedisOperationException (const char * msg_cstr) {std::string msg (msg_cstr);int_msg = msg;}
  RedisOperationException () {}
  ~RedisOperationException() throw () {}
  const char * what() const throw ()
  {
    std::string what_str = "Error Performing Redis Operation: " + int_msg;
    return what_str.c_str();
  }
};

//! A Structure for storing Redis Connection Information
struct RedisConnChain
{
  std::string ip;
  int port;
  std::string password;
  int pool_size;
  int timeout;
  int role;
};

//! The XRedis Admin

//! The XRedis Admin is responsible for all interactions with the Redis Key-Value
//! Store.  This is capable of connecting to single Redis instances or Clusters
class RedisInterface
{
public:

  virtual ~RedisInterface() {}

	//! Load a value from Redis
	virtual std::string load ( std::string key ) = 0;

	//! Save a value to Redis
	virtual bool save ( std::string key, std::string msg ) = 0;

	//! Does a key exist in Redis?
	virtual bool exists ( std::string key ) = 0;

	//! Delete a value from Redis
	virtual bool del ( std::string key ) = 0;

	//! Expire a value in Redis after a specified number of seconds
	virtual bool expire ( std::string key, unsigned int second) = 0;
};

#endif
