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

// Tests for Redis Admin

#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "include/redis_interface.h"
#include "include/factory_redis.h"

// Main Method

std::vector<RedisConnChain> RedisConnectionList;

RedisInterface *xRedis;

int main() {
  // Set up Redis Connection
  RedisComponentFactory redis_factory;
  xRedis = redis_factory.get_redis_interface("127.0.0.1", 6379);

  // Clear out the Keys from the last test
  xRedis->del("Test");
  xRedis->del("1");
  xRedis->del("2");
  xRedis->del("3");
  xRedis->del("counter");
  xRedis->del("list1");
  xRedis->del("test1");
  xRedis->del("test2");
  xRedis->del("test3");
  xRedis->del("test11");
  xRedis->del("test12");
  xRedis->del("test13");

  // Core Components

  // save
  assert(xRedis->save("Test", "123"));

  // exists
  assert(xRedis->exists("Test"));

  // load
  std::string strValue = xRedis->load("Test");
  assert(strValue == "123");
  std::cout << strValue << std::endl;

  // Delete
  assert(xRedis->del("Test"));
  assert(!(xRedis->exists("Test")));

  // Expire & Persist
  assert(xRedis->save("1", "123"));
  assert(xRedis->save("2", "456"));
  assert(xRedis->save("3", "789"));
  assert(xRedis->expire("1", 1));
  assert(xRedis->expire("2", 10));
  assert(xRedis->expire("3", 5));
  assert(xRedis->exists("1"));
  assert(xRedis->exists("2"));
  assert(xRedis->exists("3"));

  sleep(2);
  assert(!(xRedis->exists("1")));
  assert(xRedis->exists("2"));
  assert(xRedis->exists("3"));

  assert(xRedis->persist("3"));

  sleep(20);
  assert(!(xRedis->exists("1")));
  assert(!(xRedis->exists("2")));
  assert(xRedis->exists("3"));

  // Setex
  assert(xRedis->setex("1", "123", 1));
  assert(xRedis->setex("2", "456", 10));
  assert(xRedis->exists("1"));
  assert(xRedis->exists("2"));

  sleep(2);
  assert(!(xRedis->exists("1")));
  assert(xRedis->exists("2"));

  sleep(20);
  assert(!(xRedis->exists("1")));
  assert(!(xRedis->exists("2")));

  // Length
  assert(xRedis->len("3") == 3);

  // append
  assert(xRedis->append("3", "1011"));
  assert(xRedis->len("3") == 7);

  // Counter Methods

  // Increase
  assert(xRedis->incr("counter") == 1);
  assert(xRedis->incr("counter") == 2);

  // Decrease
  assert(xRedis->decr("counter") == 1);
  assert(xRedis->decr("counter") == 0);

  // List Methods

  // Push
  assert(xRedis->lpush("list1", "1") == 1);
  assert(xRedis->lpush("list1", "2") == 2);
  assert(xRedis->lpush("list1", "3") == 3);
  assert(xRedis->rpush("list1", "0") == 4);

  // Pop
  assert(xRedis->lpop("list1") == "3");
  assert(xRedis->rpop("list1") == "0");

  // Set
  assert(xRedis->lset("list1", "9", 1));

  // Index
  assert(xRedis->lindex("list1", 1) == "9");
  assert(xRedis->lindex("list1", 0) == "2");

  // Length
  assert(xRedis->llen("list1") == 2);

  // Insert
  assert(xRedis->lpush("list1", "0") == 3);
  assert(xRedis->linsert("list1", "4", "2", true) == 4);
  assert(xRedis->lindex("list1", 1) == "4");
  assert(xRedis->linsert("list1", "6", "2", false) == 5);
  assert(xRedis->lindex("list1", 3) == "6");

  // Trim
  assert(xRedis->ltrim("list1", 0, 1));
  assert(xRedis->llen("list1") == 2);

  // Batch Methods

  // Multi-Save
  RedisKvPair kv1;
  RedisKvPair kv2;
  RedisKvPair kv3;

  kv1.key = "test1";
  kv2.key = "test2";
  kv3.key = "test3";
  kv1.val = "first";
  kv2.val = "second";
  kv3.val = "third";

  std::vector<RedisKvPair> save_ops;
  save_ops.push_back(kv1);
  save_ops.push_back(kv2);
  save_ops.push_back(kv3);

  assert(xRedis->mset(save_ops));

  assert(xRedis->exists("test1"));
  assert(xRedis->exists("test2"));
  assert(xRedis->exists("test3"));

  // Multi-Get
  std::vector<std::string> ret_keys;
  ret_keys.push_back("test1");
  ret_keys.push_back("test2");
  ret_keys.push_back("test3");
  ret_keys.push_back("not_there");

  std::vector<std::string> vals;
  vals = xRedis->mget(ret_keys);

  assert(vals[0] == "first");
  assert(vals[1] == "second");
  assert(vals[2] == "third");
  assert(vals[3] == "");

  // Multi-Set NX
  RedisKvPair kv11;
  RedisKvPair kv12;
  RedisKvPair kv13;

  kv11.key = "test11";
  kv12.key = "test12";
  kv13.key = "test13";
  kv11.val = "first";
  kv12.val = "second";
  kv13.val = "third";

  std::vector<RedisKvPair> save_ops2;
  save_ops2.push_back(kv11);
  save_ops2.push_back(kv12);
  save_ops2.push_back(kv13);

  assert(xRedis->msetnx(save_ops2));
  assert(xRedis->load("test11") == "first");
  assert(xRedis->load("test12") == "second");
  assert(xRedis->load("test13") == "third");

  delete xRedis;
  return 0;
}
