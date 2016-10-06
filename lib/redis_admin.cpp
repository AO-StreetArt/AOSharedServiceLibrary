#include "include/redis_admin.h"


//TO-DO: When an operation fails, it should automatically detect whether there are
//Sentinels available and, if so, try to failover to another Redis Node.


void RedisAdmin::init(std::string hostname, int port, int timeout_seconds, int timeout_microseconds)
{
  struct timeval timeout = {timeout_seconds, timeout_microseconds};
  c = redisConnectWithTimeout(hostname.c_str(), port, timeout);

  if (c == NULL || c->err) {
    if (c == NULL)
    {
      throw RedisConnectionException( "Error: Cannot Allocate Redis Instance" );
    }
    else if (c->err)
    {
      std::string err_msg (c->errstr);
      err_msg = "Error:" + err_msg;
      throw RedisConnectionException( err_msg );
    }
  exit(1);
  }
}

bool RedisAdmin::process_std_string_reply(redisReply *reply)
{
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

bool RedisAdmin::process_std_int_reply(redisReply *reply)
{
  int reply_code = reply->integer;
  freeReplyObject(reply);
  reply = NULL;
  if (reply_code == 1) {
    return true;
  }
  return false;
}

int RedisAdmin::return_int_reply(redisReply *reply)
{
  int reply_code = reply->integer;
  freeReplyObject(reply);
  reply = NULL;
  return reply_code;
}

std::string RedisAdmin::return_string_reply(redisReply *reply)
{
  std::string reply_str (reply->str);
  freeReplyObject(reply);
  reply = NULL;
  return reply_str;
}

RedisAdmin::RedisAdmin(std::string hostname, int port)
{
  init(hostname, port, 5, 0);
}

RedisAdmin::RedisAdmin(std::string hostname, int port, int timeout_seconds, int timeout_microseconds)
{
  init(hostname, port, timeout_seconds, timeout_microseconds);
}

RedisAdmin::RedisAdmin(RedisConnChain current_connection)
{
  //Connect to the DB
  init(current_connection.ip, current_connection.port, current_connection.timeout, 0);

  //Authenticate with the DB
  std::string key_str = "AUTH " + current_connection.password;
  reply = (redisReply *) redisCommand(c, key_str.c_str());
  if ( !(strcmp(reply->str, "OK") == 0) ) {
    std::string err_msg = "Error: Authentication Failed";
    throw RedisConnectionException( err_msg );
  }
  freeReplyObject(reply);

}

RedisAdmin::~RedisAdmin()
{
  if (c)
  {
    redisFree(c);
  }
}

//! Load a value from Redis
std::string RedisAdmin::load ( std::string key )
{
  std::string key_str = "GET " + key;
  reply = (redisReply *) redisCommand(c, key_str.c_str());
  return return_string_reply(reply);
}

//! Save a value to Redis
bool RedisAdmin::save ( std::string key, std::string msg )
{
  reply = (redisReply *) redisCommand( c, "SET %s %s", key.c_str(), msg.c_str() );
  return process_std_string_reply(reply);
}

//! Does a key exist in Redis?
bool RedisAdmin::exists ( std::string key )
{
  std::string key_str = "EXISTS " + key;
  reply = (redisReply *) redisCommand(c, key_str.c_str());
  return process_std_int_reply(reply);
}

//! Delete a value from Redis
bool RedisAdmin::del ( std::string key )
{
  std::string key_str = "DEL " + key;
  reply = (redisReply *) redisCommand( c, key_str.c_str() );
  return process_std_int_reply(reply);
}

//! Expire a value in Redis after a specified number of seconds
bool RedisAdmin::expire ( std::string key, unsigned int second)
{
  std::string length_key = std::to_string(second);
  reply = (redisReply *) redisCommand( c, "EXPIRE %s %s", key.c_str(), length_key.c_str() );
  return process_std_int_reply(reply);
}

bool RedisAdmin::persist ( std::string key )
{
  std::string key_str = "PERSIST " + key;
  reply = (redisReply *) redisCommand( c, key_str.c_str() );
  return process_std_int_reply(reply);
}

int RedisAdmin::incr ( std::string key )
{
  std::string key_str = "INCR " + key;
  reply = (redisReply *) redisCommand( c, key_str.c_str() );
  return return_int_reply(reply);
}

int RedisAdmin::incr ( std::string key, int incr_amt )
{
  std::string key_str = "INCRBY " + key + " " + std::to_string(incr_amt);
  reply = (redisReply *) redisCommand( c, key_str.c_str() );
  return return_int_reply(reply);
}

int RedisAdmin::decr ( std::string key )
{
  std::string key_str = "DECR " + key;
  reply = (redisReply *) redisCommand( c, key_str.c_str() );
  return return_int_reply(reply);
}

int RedisAdmin::decr ( std::string key, int decr_amt )
{
  std::string key_str = "DECRBY " + key + " " + std::to_string(decr_amt);
  reply = (redisReply *) redisCommand( c, key_str.c_str() );
  return return_int_reply(reply);
}

bool RedisAdmin::setex ( std::string key, std::string val, unsigned int second)
{
  std::string length_key = std::to_string(second);
  reply = (redisReply *) redisCommand( c, "SETEX %s %s %s", key.c_str(), length_key.c_str(), val.c_str() );
  return process_std_string_reply(reply);
}

bool RedisAdmin::append ( std::string key, std::string val )
{
  reply = (redisReply *) redisCommand( c, "APPEND %s %s", key.c_str(), val.c_str() );
  return return_int_reply(reply);
}

int RedisAdmin::len ( std::string key )
{
  std::string key_str = "STRLEN " + key;
  reply = (redisReply *) redisCommand( c, key_str.c_str() );
  return return_int_reply(reply);
}

int RedisAdmin::lpush ( std::string key, std::string val )
{
  reply = (redisReply *) redisCommand( c, "LPUSH %s %s", key.c_str(), val.c_str() );
  return return_int_reply(reply);
}

int RedisAdmin::rpush ( std::string key, std::string val )
{
  reply = (redisReply *) redisCommand( c, "RPUSH %s %s", key.c_str(), val.c_str() );
  return return_int_reply(reply);
}

//! Pop a value from a Redis list on the given key
std::string RedisAdmin::lpop ( std::string key )
{
  std::string key_str = "LPOP " + key;
  reply = (redisReply *) redisCommand(c, key_str.c_str());
  return return_string_reply(reply);
}

//! Pop a value from a Redis list on the given key
std::string RedisAdmin::rpop ( std::string key )
{
  std::string key_str = "RPOP " + key;
  reply = (redisReply *) redisCommand(c, key_str.c_str());
  return return_string_reply(reply);
}

//! Set the value stored in the list at key and the index at index
bool RedisAdmin::lset ( std::string key, std::string val, int index)
{
  std::string index_str = std::to_string(index);
  reply = (redisReply *) redisCommand( c, "LSET %s %s %s", key.c_str(), index_str.c_str(), val.c_str() );
  return process_std_string_reply(reply);
}

//! Insert a value into the list at key and before/after the pivot value
int RedisAdmin::linsert ( std::string key, std::string val, std::string pivot, bool before_pivot)
{
  std::string ba_str;
  if (before_pivot) {ba_str = "BEFORE";}
  else {ba_str = "AFTER";}
  reply = (redisReply *) redisCommand(c, "LINSERT %s %s %s %s", key.c_str(), ba_str.c_str(), pivot.c_str(), val.c_str());
  return return_int_reply(reply);
}

//! Get the value stored in the list at key and the index at index
std::string RedisAdmin::lindex ( std::string key, int index)
{
  std::string length_key = std::to_string(index);
  reply = (redisReply *) redisCommand( c, "LINDEX %s %s", key.c_str(), length_key.c_str() );
  return return_string_reply(reply);
}

//! Get the length of a list stored in Redis
int RedisAdmin::llen ( std::string key )
{
  std::string key_str = "LLEN " + key;
  reply = (redisReply *) redisCommand( c, key_str.c_str() );
  return return_int_reply(reply);
}

//! Trim a list to the specified start and end index
bool RedisAdmin::ltrim ( std::string key, int start_index, int end_index)
{
  std::string start = std::to_string(start_index);
  std::string end = std::to_string(end_index);
  reply = (redisReply *) redisCommand( c, "LTRIM %s %s %s", key.c_str(), start.c_str(), end.c_str() );
  return process_std_string_reply(reply);
}

bool RedisAdmin::mset ( std::vector<RedisKvPair> save_sets )
{
  std::string cmd_str = "MSET ";
  for (std::size_t i=0; i<save_sets.size(); ++i)
  {
    cmd_str = cmd_str + save_sets[i].key + " " + save_sets[i].val + " ";
  }

  reply = (redisReply *) redisCommand( c, cmd_str.c_str() );
  return process_std_string_reply(reply);
}

std::vector<std::string> RedisAdmin::mget ( std::vector<std::string> keys )
{
  std::string cmd_str = "MGET ";
  for (std::size_t i=0; i<keys.size(); ++i)
  {
    cmd_str = cmd_str + keys[i] + " ";
  }

  reply = (redisReply *) redisCommand( c, cmd_str.c_str() );
  std::vector<std::string> reply_string;

  for (std::size_t j=0;j<reply->elements; ++j)
  {
    if (reply->element[j]->str) {
      std::string new_element (reply->element[j]->str);
      reply_string.push_back(new_element);
    }
    else {
      std::string new_element = "";
      reply_string.push_back(new_element);
    }
  }

  return reply_string;
}

bool RedisAdmin::msetnx ( std::vector<RedisKvPair> save_sets )
{
  std::string cmd_str = "MSETNX ";
  for (std::size_t i=0; i<save_sets.size(); ++i)
  {
    cmd_str = cmd_str + save_sets[i].key + " " + save_sets[i].val + " ";
  }

  reply = (redisReply *) redisCommand( c, cmd_str.c_str() );
  return process_std_int_reply(reply);
}

bool RedisAdmin::setnx ( std::string key, std::string msg )
{
  reply = (redisReply *) redisCommand( c, "SETNX %s %s", key.c_str(), msg.c_str() );
  return process_std_int_reply(reply);
}
