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

#include <stdlib.h>
#include <cstdlib>
#include <sstream>
#include <string>
#include <exception>

#include "aossl/redis/include/redis_interface.h"
#include "app_log.h"

#ifndef EXAMPLES_ZMQ_SHELL_SRC_INCLUDE_REDIS_LOCKING_H_
#define EXAMPLES_ZMQ_SHELL_SRC_INCLUDE_REDIS_LOCKING_H_

class RedisLocker {
  RedisInterface *redis = NULL;

 public:
  RedisLocker(RedisInterface *r) {redis = r;}
  ~RedisLocker() {}

  // Establish a Redis Mutex Lock on a given key
  inline void get_lock(std::string key, std::string val) {
    // Determine if another instance of CLyman has a lock on the Redis Mutex
    std::string current_mutex_key;
    bool lock_established = false;

    while (!lock_established) {
      redis_logging->error("Redis Mutex Lock Routine Started");

      if (redis->exists(key)) {
        try {
          current_mutex_key = redis->load(key);
        }
        catch (std::exception& e) {
          redis_logging->error("Exception encountered during Redis Request");
          redis_logging->error(e.what());
        }
      }

      if (((current_mutex_key != "") && \
        (current_mutex_key != val)) || val.empty())  {
        // Another instance of Clyman has a lock on the redis mutex
        // Block until the lock is cleared
        redis_logging->error("Existing Redis Mutex Lock Detected, waiting");
        while (redis->exists(key)) {}
      }

      // Try to establish a lock on the Redis Mutex
      redis_logging->error("Attempting to obtain Redis Mutex Lock");
      if (!(redis->exists(key))) {
        try {
          lock_established = redis->setnx(key, val);
        }
        catch (std::exception& e) {
          redis_logging->error("Exception encountered during Redis Request");
          redis_logging->error(e.what());
        }
      }
    }
  }

  void get_lock(std::string key) {get_lock(key, "");}

  bool release_lock(std::string key, std::string val) {
    std::string current_mutex_key = "";
    if (redis->exists(key)) {
      current_mutex_key = redis->load(key);
    }
    if (val.empty() || (val == current_mutex_key)) {
      return redis->del(key);
    }
    return false;
  }
};

#endif  // EXAMPLES_ZMQ_SHELL_SRC_INCLUDE_REDIS_LOCKING_H_
