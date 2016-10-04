//An admin using the xRedis C++ Drivers for Redis

#include <string.h>
#include <string>
#include <vector>

#ifndef REDIS_INTERFACE
#define REDIS_INTERFACE

//----------------------------------------------------------------------------//
//----------------------------------Errors------------------------------------//
//----------------------------------------------------------------------------//

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

//----------------------------------------------------------------------------//
//------------------------------Core Elements---------------------------------//
//----------------------------------------------------------------------------//

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

struct RedisKvPair
{
  std::string key;
  std::string val;
};

//! The Redis Admin

//! The Redis Admin is responsible for all interactions with the Redis Key-Value
//! Store
class RedisInterface
{
public:

  virtual ~RedisInterface() {}

//----------------------------------------------------------------------------//
//---------------------------Standard Operations------------------------------//
//----------------------------------------------------------------------------//

	//! Load a value from Redis
	virtual std::string load ( std::string key ) = 0;

	//! Save a value to Redis
	virtual bool save ( std::string key, std::string msg ) = 0;

  //! Set a value in Redis, only if the key does not exist
	virtual bool setnx ( std::string key, std::string msg ) = 0;

	//! Does a key exist in Redis?
	virtual bool exists ( std::string key ) = 0;

	//! Delete a value from Redis
	virtual bool del ( std::string key ) = 0;

	//! Expire a value in Redis after a specified number of seconds
	virtual bool expire ( std::string key, unsigned int second) = 0;

  //! Prevent a value from being expired in Redis
	virtual bool persist ( std::string key ) = 0;

  //! Set and add an expiration to an object in a single operation
  virtual bool setex ( std::string key, std::string val, unsigned int second) = 0;

  //! Append a value to a Redis value on the given key
  virtual bool append ( std::string key, std::string val ) = 0;

  //! Return the length of the string value stored at key
  virtual int len ( std::string key ) = 0;

//----------------------------------------------------------------------------//
//----------------------------Counter Operations------------------------------//
//----------------------------------------------------------------------------//

  //! Increment a Counter value in Redis
  virtual int incr ( std::string key ) = 0;

  //! Increment a Counter value in Redis
  virtual int incr ( std::string key, int incr_amt ) = 0;

  //! Decriment a Counter value in Redis
  virtual int decr ( std::string key ) = 0;

  //! Decriment a Counter value in Redis
  virtual int decr ( std::string key, int decr_amt ) = 0;

//----------------------------------------------------------------------------//
//-----------------------------List Operations--------------------------------//
//----------------------------------------------------------------------------//

  //! Push a value to a Redis list on the given key
	virtual int lpush ( std::string key, std::string val ) = 0;

  //! Push a value to a Redis list on the given key
  virtual int rpush ( std::string key, std::string val ) = 0;

  //! Pop a value from a Redis list on the given key
	virtual std::string lpop ( std::string key ) = 0;

  //! Pop a value from a Redis list on the given key
  virtual std::string rpop ( std::string key ) = 0;

  //! Set the value stored in the list at key and the index at index
  virtual bool lset ( std::string key, std::string val, int index) = 0;

  //! Insert a value into the list at key and before/after the pivot value
  virtual int linsert ( std::string key, std::string val, std::string pivot, bool before_pivot=true) = 0;

  //! Get the value stored in the list at key and the index at index
  virtual std::string lindex ( std::string key, int index) = 0;

  //! Get the length of a list stored in Redis
  virtual int llen ( std::string key ) = 0;

  //! Trim a list to the specified start and end index
  virtual bool ltrim ( std::string key, int start_index, int end_index) = 0;

//----------------------------------------------------------------------------//
//-----------------------------Bulk Operations--------------------------------//
//----------------------------------------------------------------------------//

  //! Get a set of string replies
  virtual std::vector<std::string> mget ( std::vector<std::string> keys ) = 0;

  //! Set a set of string objects
  virtual bool mset ( std::vector<RedisKvPair> save_sets ) = 0;

  //! Set a set of string objects, as long as none of the keys exist
  virtual bool msetnx ( std::vector<RedisKvPair> save_sets ) = 0;
};

#endif
