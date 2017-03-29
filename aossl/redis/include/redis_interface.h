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

//An admin using the xRedis C++ Drivers for Redis

#ifndef REDIS_INTERFACE
#define REDIS_INTERFACE

#include <string.h>
#include <string>
#include <vector>

//----------------------------------------------------------------------------//
//----------------------------------Errors------------------------------------//
//----------------------------------------------------------------------------//

//! An Implementation of std::exception that denotes a connection error within Redis
struct RedisConnectionException: public std::exception
{
  //! An error message passed on initialization
  std::string int_msg;
  const char * what_str;

  //! Create a Redis Connection Exception, and store the given error message
  RedisConnectionException (std::string msg) {int_msg = "Error Connecting to Redis: " + msg;what_str=int_msg.c_str();}

  //! Create a Redis Connection Exception, and store the given error message
  RedisConnectionException (const char * msg_cstr) {std::string msg (msg_cstr);int_msg = msg;}

  RedisConnectionException () {}
  ~RedisConnectionException() throw () {}

  //! Show the error message in readable format
  const char * what() const throw ()
  {
    std::string what_str = "Error Connecting to Redis: " + int_msg;
    return what_str.c_str();
  }
};

//! An Implementation of std::exception that denotes an error within Redis during a transaction
struct RedisOperationException: public std::exception
{
  //! An error message passed on initialization
  std::string int_msg;

  //! Create a Redis Operation Exception, and store the given error message
  RedisOperationException (std::string msg) {int_msg = msg;}

  //! Create a Redis Operation Exception, and store the given error message
  RedisOperationException (const char * msg_cstr) {std::string msg (msg_cstr);int_msg = msg;}

  RedisOperationException () {}
  ~RedisOperationException() throw () {}

  //! Show the error message in readable format
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
  //! The IP of the Redis Node
  std::string ip;

  //! The port of the Redis Node
  int port;

  //! The password of the Redis Node
  std::string password;

  //! The pool size on the Redis Node
  int pool_size;

  //! The timeout on the Redis Node
  int timeout;

  //! The Role on the Redis Node
  int role;
};

//! A Structure for storing a Key-Value pair, used with batch operations
struct RedisKvPair
{
  //! Key of the pair
  std::string key;

  //! Value stored in the pair
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
  virtual int append ( std::string key, std::string val ) = 0;

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
  virtual int linsert ( std::string key, std::string val, std::string pivot, bool before_pivot) = 0;

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
