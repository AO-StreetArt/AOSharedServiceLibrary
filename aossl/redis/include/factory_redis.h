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

#ifndef AOSSL_FACTORY_REDIS
#define AOSSL_FACTORY_REDIS

#include <string.h>
#include "redis_interface.h"
#include "redis_admin.h"

//! The Redis Service Component Factory

//! The Service Component Factory tracks the Redis
//! objects exposed by the framework and passes back
//! instances of interfaces.  This allows for the publicly exposed methods
//! to be independent of the implementations.
class RedisComponentFactory {

 public:

  //! Create a new Service Component Factory
  RedisComponentFactory() {}

  //! Delete a Service Component Factory
  ~RedisComponentFactory() {}

  //! Get a Redis Interface Instance
  inline RedisInterface* get_redis_interface(std::string hostname, int port, int timeout_seconds, int timeout_microseconds) {
    return new RedisAdmin( hostname, port, timeout_seconds, timeout_microseconds);
  }

  //! Get a Redis Interface Instance
  inline RedisInterface* get_redis_interface(std::string hostname, int port) {
    return new RedisAdmin( hostname, port);
  }

  //! Get a Redis Interface Instance
  inline RedisInterface* get_redis_interface(std::string hostname, int port, int pool_size) {
    return new RedisAdmin( hostname, port, pool_size);
  }

  //! Get a Redis Interface Instance
  inline RedisInterface* get_redis_interface(std::string hostname, int port, int timeout_seconds, int timeout_microseconds, int pool_size) {
    return new RedisAdmin( hostname, port, timeout_seconds, timeout_microseconds, pool_size);
  }

  //! Get a Redis Interface Instance
  inline RedisInterface* get_redis_interface(RedisConnChain connection_list) {
    return new RedisAdmin( connection_list );
  }

  //! Get a Redis Interface Instance
  inline RedisInterface* get_redis_interface(RedisConnChain connection_list, int pool_size) {
    return new RedisAdmin( connection_list, pool_size );
  }

};

#endif
