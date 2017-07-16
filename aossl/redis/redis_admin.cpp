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

#include "include/redis_admin.h"

const char * NULL_PASSWORD = "";


// Connection Pool

void RedisConnectionPool::init_slots() {
  slot_pool = new AOSSL::SlotPool(connection_limit);
}

// Initialize the connections required to start the connection pool
void RedisConnectionPool::init_connections(const char * conn_str, \
  const char * passwd, int con_port, int timeout_secs, int timeout_microsecs) {
  connection_string.assign(conn_str);
  port = con_port;
  timeout_seconds = timeout_secs;
  timeout_microseconds = timeout_microsecs;

  if (passwd) {
    if (passwd[0] != '\0') {
      std::string new_pass(passwd);
      password = new_pass;
    }
  }

  for (int i = 0; i < start_connections; i++) {
    RedisSession rs;

    struct timeval timeout = {timeout_secs, timeout_microsecs};
    rs.connection = redisConnectWithTimeout(conn_str, con_port, timeout);

    if (!(rs.connection)) {
      throw RedisConnectionException("Error: Cannot Allocate Redis Instance");
    } else if (rs.connection->err) {
      err_msg.assign(rs.connection->errstr);
      err_msg = "Error:" + err_msg;
      throw RedisConnectionException(err_msg);
    }

    // Check if we've been given a password to authenticate with
    if (passwd) {
      if (passwd[0] != '\0') {
        // Authenticate the connection
        std::string key_str(passwd);
        key_str = "AUTH " + key_str;
        redisReply *reply = \
          (redisReply *) redisCommand(rs.connection, key_str.c_str());
        if ( !(strcmp(reply->str, "OK") == 0) ) {
          err_msg.assign("Error: Authentication Failed");
          throw RedisConnectionException(err_msg);
        }
        freeReplyObject(reply);
      }
    }

    connections.push_back(rs);
  }
  current_max_connection = start_connections;
}

// Clean up the connection pool
RedisConnectionPool::~RedisConnectionPool() {
  for (int i = 0; i < current_max_connection; i++) {
    // Free the redis instance
    if (connections[i].connection) {
      redisFree(connections[i].connection);
    }
  }
  delete slot_pool;
}

// Retrieve a connection
RedisSession* RedisConnectionPool::get_connection() {
  // Find the next available connection slot
  // If none are available, wait until one is freed
  int current_connection = slot_pool->find_next_slot();

  // Create new connections if necessary
  if (current_connection > current_max_connection) {
    std::lock_guard<std::mutex> lock(create_conn_mutex);
    if (current_connection > current_max_connection) {
      const char * con_cstr = connection_string.c_str();
      for (
        int i = current_max_connection;
        i < current_max_connection + connection_creation_batch;
        i++
      ) {
        RedisSession rs;

        struct timeval timeout = {timeout_seconds, timeout_microseconds};
        rs.connection = redisConnectWithTimeout(con_cstr, port, timeout);

        if (rs.connection == NULL || rs.connection->err) {
          if (rs.connection == NULL) {
            throw RedisConnectionException("Error: Cannot Create Redis Instance");
          } else if (rs.connection->err) {
            std::string err_msg(rs.connection->errstr);
            err_msg = "Error:" + err_msg;
            throw RedisConnectionException(err_msg);
          }
        }

        connections.push_back(rs);
      }
      current_max_connection = current_max_connection+connection_creation_batch;
    }
  }

  // Pack the connection & session into the return object
  RedisSession *s = new RedisSession;
  s->connection = connections[current_connection].connection;
  s->index = current_connection;

  // Return the latest connection information
  // When we leave the method, the mutex lock is released automatically
  return s;
}

// Release a connection for re-use
void RedisConnectionPool::release_connection(RedisSession *conn) {
  // Release the slot
  slot_pool->release_slot(conn->index);
  // Delete the query session pointer, but not it's contents
  delete conn;
}

// Redis Admin

void RedisAdmin::init(std::string hostname, std::string passwd, int port, \
  int timeout_seconds, int timeout_microseconds, int pool_size) {
  int_hostname = hostname;
  if (passwd.empty()) {
    pool = new RedisConnectionPool(pool_size, int_hostname.c_str(), NULL, \
      port, timeout_seconds, timeout_microseconds, 1, 1);
  } else {
    pool = new RedisConnectionPool(pool_size, int_hostname.c_str(), \
      passwd.c_str(), port, timeout_seconds, timeout_microseconds, 1, 1);
  }
}

bool RedisAdmin::process_std_string_reply(redisReply *reply) {
  bool ret_val = false;
  if (reply->str) {
    if ( strcmp(reply->str, "OK") == 0 ) {
      ret_val = true;
    }
  }
  freeReplyObject(reply);
  reply = NULL;
  return ret_val;
}

bool RedisAdmin::process_std_int_reply(redisReply *reply) {
  int reply_code = reply->integer;
  freeReplyObject(reply);
  reply = NULL;
  if (reply_code == 1) {
    return true;
  }
  return false;
}

int RedisAdmin::return_int_reply(redisReply *reply) {
  int reply_code = reply->integer;
  freeReplyObject(reply);
  reply = NULL;
  return reply_code;
}

void RedisAdmin::return_string_reply(redisReply *reply) {
  if (reply->str) {
    reply_str.assign(reply->str);
    freeReplyObject(reply);
    reply = NULL;
  } else {
    freeReplyObject(reply);
    reply = NULL;
    reply_str.assign("");
  }
}

AOSSL::StringBuffer* RedisAdmin::return_stringbuf_reply(redisReply *reply) {
  std::string reply_string;
  AOSSL::StringBuffer *buf = new AOSSL::StringBuffer;
  if (reply->str) {
    reply_string.assign(reply->str);
    freeReplyObject(reply);
    reply = NULL;
    buf->success = true;
    std::string empty = "";
    buf->err_msg = empty;
  } else {
    freeReplyObject(reply);
    reply = NULL;
    reply_string.assign("");
    buf->success = false;
    std::string error_string = "No reply found";
    buf->err_msg = error_string;
  }
  buf->val = reply_string;
  return buf;
}

RedisAdmin::RedisAdmin(std::string hostname, int port) {
  init(hostname, "", port, 5, 0, 5);
}

RedisAdmin::RedisAdmin(std::string hostname, int port, int pool_size) {
  init(hostname, "", port, 5, 0, pool_size);
}

RedisAdmin::RedisAdmin(std::string hostname, int port, int timeout_seconds, \
  int timeout_microseconds) {
  init(hostname, "", port, timeout_seconds, timeout_microseconds, 5);
}

RedisAdmin::RedisAdmin(std::string hostname, int port, int timeout_seconds, \
  int timeout_microseconds, int pool_size) {
  init(hostname, "", port, timeout_seconds, timeout_microseconds, pool_size);
}

RedisAdmin::RedisAdmin(RedisConnChain current_connection) {
  // Connect
  init(current_connection.ip, current_connection.password, \
    current_connection.port, current_connection.timeout, 0, 5);
}

RedisAdmin::RedisAdmin(RedisConnChain current_connection, int pool_size) {
  // Connect
  init(current_connection.ip, current_connection.password, \
    current_connection.port, current_connection.timeout, 0, pool_size);
}

// Load a value from Redis
std::string RedisAdmin::load(std::string key) {
  RedisSession *rs = pool->get_connection();
  std::string key_str = "GET " + key;
  redisReply *reply = \
    (redisReply *) redisCommand(rs->connection, key_str.c_str());
  return_string_reply(reply);
  pool->release_connection(rs);
  return reply_str;
}

// Load a value from Redis
AOSSL::StringBuffer* RedisAdmin::load_safe(std::string key) {
  RedisSession *rs = pool->get_connection();
  std::string key_str = "GET " + key;
  redisReply *reply = \
    (redisReply *) redisCommand(rs->connection, key_str.c_str());
  AOSSL::StringBuffer *buf = return_stringbuf_reply(reply);
  pool->release_connection(rs);
  return buf;
}

// Save a value to Redis
bool RedisAdmin::save(std::string key, std::string msg) {
  RedisSession *rs = pool->get_connection();
  redisReply *reply = (redisReply *) redisCommand(rs->connection, \
    "SET %s %s", key.c_str(), msg.c_str());
  bool resp = process_std_string_reply(reply);
  pool->release_connection(rs);
  return resp;
}

// Does a key exist in Redis?
bool RedisAdmin::exists(std::string key) {
  RedisSession *rs = pool->get_connection();
  std::string key_str = "EXISTS " + key;
  redisReply *reply = \
    (redisReply *) redisCommand(rs->connection, key_str.c_str());
  bool resp = process_std_int_reply(reply);
  pool->release_connection(rs);
  return resp;
}

// Delete a value from Redis
bool RedisAdmin::del(std::string key) {
  RedisSession *rs = pool->get_connection();
  std::string key_str = "DEL " + key;
  redisReply *reply = \
    (redisReply *) redisCommand(rs->connection, key_str.c_str());
  bool resp = process_std_int_reply(reply);
  pool->release_connection(rs);
  return resp;
}

// Expire a value in Redis after a specified number of seconds
bool RedisAdmin::expire(std::string key, unsigned int second) {
  RedisSession *rs = pool->get_connection();
  std::string length_key = std::to_string(second);
  redisReply *reply = (redisReply *) redisCommand(rs->connection, \
    "EXPIRE %s %s", key.c_str(), length_key.c_str());
  bool resp = process_std_int_reply(reply);
  pool->release_connection(rs);
  return resp;
}

bool RedisAdmin::persist(std::string key) {
  RedisSession *rs = pool->get_connection();
  std::string key_str = "PERSIST " + key;
  redisReply *reply = \
    (redisReply *) redisCommand(rs->connection, key_str.c_str());
  bool resp = process_std_int_reply(reply);
  pool->release_connection(rs);
  return resp;
}

int RedisAdmin::incr(std::string key) {
  RedisSession *rs = pool->get_connection();
  std::string key_str = "INCR " + key;
  redisReply *reply = \
    (redisReply *) redisCommand(rs->connection, key_str.c_str());
  int resp = return_int_reply(reply);
  pool->release_connection(rs);
  return resp;
}

int RedisAdmin::incr(std::string key, int incr_amt) {
  RedisSession *rs = pool->get_connection();
  std::string key_str = "INCRBY " + key + " " + std::to_string(incr_amt);
  redisReply *reply = \
    (redisReply *) redisCommand(rs->connection, key_str.c_str());
  int resp = return_int_reply(reply);
  pool->release_connection(rs);
  return resp;
}

int RedisAdmin::decr(std::string key) {
  RedisSession *rs = pool->get_connection();
  std::string key_str = "DECR " + key;
  redisReply *reply = \
    (redisReply *) redisCommand(rs->connection, key_str.c_str());
  int resp = return_int_reply(reply);
  pool->release_connection(rs);
  return resp;
}

int RedisAdmin::decr(std::string key, int decr_amt) {
  RedisSession *rs = pool->get_connection();
  std::string key_str = "DECRBY " + key + " " + std::to_string(decr_amt);
  redisReply *reply = \
    (redisReply *) redisCommand(rs->connection, key_str.c_str());
  int resp = return_int_reply(reply);
  pool->release_connection(rs);
  return resp;
}

bool RedisAdmin::setex(std::string key, std::string val, unsigned int second) {
  RedisSession *rs = pool->get_connection();
  std::string length_key = std::to_string(second);
  redisReply *reply = (redisReply *) redisCommand(rs->connection, \
    "SETEX %s %s %s", key.c_str(), length_key.c_str(), val.c_str());
  bool resp = process_std_string_reply(reply);
  pool->release_connection(rs);
  return resp;
}

int RedisAdmin::append(std::string key, std::string val) {
  RedisSession *rs = pool->get_connection();
  redisReply *reply = (redisReply *) redisCommand(rs->connection, \
    "APPEND %s %s", key.c_str(), val.c_str());
  int resp = return_int_reply(reply);
  pool->release_connection(rs);
  return resp;
}

int RedisAdmin::len(std::string key) {
  RedisSession *rs = pool->get_connection();
  std::string key_str = "STRLEN " + key;
  redisReply *reply = \
    (redisReply *) redisCommand(rs->connection, key_str.c_str());
  int resp = return_int_reply(reply);
  pool->release_connection(rs);
  return resp;
}

int RedisAdmin::lpush(std::string key, std::string val) {
  RedisSession *rs = pool->get_connection();
  redisReply *reply = (redisReply *) redisCommand(rs->connection, \
    "LPUSH %s %s", key.c_str(), val.c_str());
  int resp = return_int_reply(reply);
  pool->release_connection(rs);
  return resp;
}

int RedisAdmin::rpush(std::string key, std::string val) {
  RedisSession *rs = pool->get_connection();
  redisReply *reply = (redisReply *) redisCommand(rs->connection, \
    "RPUSH %s %s", key.c_str(), val.c_str());
  int resp = return_int_reply(reply);
  pool->release_connection(rs);
  return resp;
}

// Pop a value from a Redis list on the given key
std::string RedisAdmin::lpop(std::string key) {
  RedisSession *rs = pool->get_connection();
  std::string key_str = "LPOP " + key;
  redisReply *reply = \
    (redisReply *) redisCommand(rs->connection, key_str.c_str());
  return_string_reply(reply);
  pool->release_connection(rs);
  return reply_str;
}

// Pop a value from a Redis list on the given key
AOSSL::StringBuffer* RedisAdmin::lpop_safe(std::string key) {
  RedisSession *rs = pool->get_connection();
  std::string key_str = "LPOP " + key;
  redisReply *reply = \
    (redisReply *) redisCommand(rs->connection, key_str.c_str());
  AOSSL::StringBuffer *buf = return_stringbuf_reply(reply);
  pool->release_connection(rs);
  return buf;
}

// Pop a value from a Redis list on the given key
std::string RedisAdmin::rpop(std::string key) {
  RedisSession *rs = pool->get_connection();
  std::string key_str = "RPOP " + key;
  redisReply *reply = \
    (redisReply *) redisCommand(rs->connection, key_str.c_str());
  return_string_reply(reply);
  pool->release_connection(rs);
  return reply_str;
}

// Pop a value from a Redis list on the given key
AOSSL::StringBuffer* RedisAdmin::rpop_safe(std::string key) {
  RedisSession *rs = pool->get_connection();
  std::string key_str = "RPOP " + key;
  redisReply *reply = \
    (redisReply *) redisCommand(rs->connection, key_str.c_str());
  AOSSL::StringBuffer *buf = return_stringbuf_reply(reply);
  pool->release_connection(rs);
  return buf;
}

// Set the value stored in the list at key and the index at index
bool RedisAdmin::lset(std::string key, std::string val, int index) {
  RedisSession *rs = pool->get_connection();
  std::string index_str = std::to_string(index);
  redisReply *reply = (redisReply *) redisCommand(rs->connection, \
    "LSET %s %s %s", key.c_str(), index_str.c_str(), val.c_str());
  bool resp_string = process_std_string_reply(reply);
  pool->release_connection(rs);
  return resp_string;
}

// Insert a value into the list at key and before/after the pivot value
int RedisAdmin::linsert(std::string key, std::string val, std::string pivot, \
  bool before_pivot) {
  RedisSession *rs = pool->get_connection();
  std::string ba_str;
  if (before_pivot) {ba_str = "BEFORE";} else {ba_str = "AFTER";}
  redisReply *reply = (redisReply *) redisCommand(rs->connection, \
    "LINSERT %s %s %s %s", key.c_str(), ba_str.c_str(), \
    pivot.c_str(), val.c_str());
  int resp = return_int_reply(reply);
  pool->release_connection(rs);
  return resp;
}

// Get the value stored in the list at key and the index at index
std::string RedisAdmin::lindex(std::string key, int index) {
  RedisSession *rs = pool->get_connection();
  std::string length_key = std::to_string(index);
  redisReply *reply = (redisReply *) redisCommand(rs->connection, \
    "LINDEX %s %s", key.c_str(), length_key.c_str());
  return_string_reply(reply);
  pool->release_connection(rs);
  return reply_str;
}

// Get the value stored in the list at key and the index at index
AOSSL::StringBuffer* RedisAdmin::lindex_safe(std::string key, int index) {
  RedisSession *rs = pool->get_connection();
  std::string length_key = std::to_string(index);
  redisReply *reply = (redisReply *) redisCommand(rs->connection, \
    "LINDEX %s %s", key.c_str(), length_key.c_str());
  AOSSL::StringBuffer *buf = return_stringbuf_reply(reply);
  pool->release_connection(rs);
  return buf;
}

// Get the length of a list stored in Redis
int RedisAdmin::llen(std::string key) {
  RedisSession *rs = pool->get_connection();
  std::string key_str = "LLEN " + key;
  redisReply *reply = \
    (redisReply *) redisCommand(rs->connection, key_str.c_str());
  int resp = return_int_reply(reply);
  pool->release_connection(rs);
  return resp;
}

// Trim a list to the specified start and end index
bool RedisAdmin::ltrim(std::string key, int start_index, int end_index) {
  RedisSession *rs = pool->get_connection();
  std::string start = std::to_string(start_index);
  std::string end = std::to_string(end_index);
  redisReply *reply = (redisReply *) redisCommand(rs->connection, \
    "LTRIM %s %s %s", key.c_str(), start.c_str(), end.c_str());
  bool resp_string = process_std_string_reply(reply);
  pool->release_connection(rs);
  return resp_string;
}

bool RedisAdmin::mset(std::vector<RedisKvPair> save_sets) {
  RedisSession *rs = pool->get_connection();
  std::string cmd_str = "MSET ";
  for (std::size_t i=0; i < save_sets.size(); ++i) {
    cmd_str = cmd_str + save_sets[i].key + " " + save_sets[i].val + " ";
  }

  redisReply *reply = \
    (redisReply *) redisCommand(rs->connection, cmd_str.c_str());
  bool resp_string = process_std_string_reply(reply);
  pool->release_connection(rs);
  return resp_string;
}

std::vector<std::string> RedisAdmin::mget(std::vector<std::string> keys) {
  RedisSession *rs = pool->get_connection();
  std::string cmd_str = "MGET ";
  for (std::size_t i=0; i < keys.size(); ++i) {
    cmd_str = cmd_str + keys[i] + " ";
  }

  redisReply *reply = \
    (redisReply *) redisCommand(rs->connection, cmd_str.c_str());
  std::vector<std::string> reply_string;

  for (std::size_t j=0; j < reply->elements; ++j) {
    if (reply->element[j]->str) {
      std::string new_element(reply->element[j]->str);
      reply_string.push_back(new_element);
    } else {
      std::string new_element = "";
      reply_string.push_back(new_element);
    }
  }
  pool->release_connection(rs);
  freeReplyObject(reply);
  return reply_string;
}

bool RedisAdmin::msetnx(std::vector<RedisKvPair> save_sets) {
  RedisSession *rs = pool->get_connection();
  std::string cmd_str = "MSETNX ";
  for (std::size_t i=0; i < save_sets.size(); ++i) {
    cmd_str = cmd_str + save_sets[i].key + " " + save_sets[i].val + " ";
  }

  redisReply *reply = \
    (redisReply *) redisCommand(rs->connection, cmd_str.c_str());
  bool resp_string = process_std_int_reply(reply);
  pool->release_connection(rs);
  return resp_string;
}

bool RedisAdmin::setnx(std::string key, std::string msg) {
  RedisSession *rs = pool->get_connection();
  redisReply *reply = (redisReply *) redisCommand(rs->connection, \
    "SETNX %s %s", key.c_str(), msg.c_str());
  bool resp_string = process_std_int_reply(reply);
  pool->release_connection(rs);
  return resp_string;
}
