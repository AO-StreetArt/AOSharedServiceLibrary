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

RedisAdmin::RedisAdmin(std::string hostname, int port)
{
  init(hostname, port, 5, 0);
}

RedisAdmin::RedisAdmin(std::string hostname, int port, int timeout_seconds, int timeout_microseconds)
{
  init(hostname, port, timeout_seconds, timeout_microseconds);
}

RedisAdmin::~RedisAdmin()
{
  if (c)
  {
    delete c;
  }
  if (reply)
  {
    delete reply;
  }
}

//! Load a value from Redis
std::string RedisAdmin::load ( std::string key )
{
  std::string key_str = "GET " + key;
  reply = (redisReply *) redisCommand(c, key_str.c_str());
  std::string reply_str (reply->str);
  freeReplyObject(reply);
  return reply_str;
}

//! Save a value to Redis
bool RedisAdmin::save ( std::string key, std::string msg )
{
  reply = (redisReply *) redisCommand( c, "SET %s %s", key.c_str(), msg.c_str() );
  freeReplyObject(reply);
  if ( strcmp(reply->str, "OK") == 0 ) {
    return true;
  }
  return false;
}

//! Does a key exist in Redis?
bool RedisAdmin::exists ( std::string key )
{
  std::string key_str = "EXISTS " + key;
  reply = (redisReply *) redisCommand(c, key_str.c_str());
  int reply_code = reply->integer;
  freeReplyObject(reply);
  if (reply_code == 1) {
    return true;
  }
  return false;
}

//! Delete a value from Redis
bool RedisAdmin::del ( std::string key )
{
  std::string key_str = "DEL " + key;
  reply = (redisReply *) redisCommand( c, key_str.c_str() );
  freeReplyObject(reply);
  if (reply->integer > 0) {
    return true;
  }
  return false;
}

//! Expire a value in Redis after a specified number of seconds
bool RedisAdmin::expire ( std::string key, unsigned int second)
{
  std::string length_key = std::to_string(second);
  reply = (redisReply *) redisCommand( c, "EXPIRE %s %s", key.c_str(), length_key.c_str() );
  freeReplyObject(reply);
  if (reply->integer == 1) {
    return true;
  }
  return false;
}
