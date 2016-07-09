#include "xredis_admin.h"

// AP Hash Function

unsigned int APHash(const char *str) {
	unsigned int hash = 0;
	int i;
	for (i=0; *str; i++) {
		if ((i&  1) == 0) {
			hash ^= ((hash << 7) ^ (*str++) ^ (hash >> 3));
		} else {
			hash ^= (~((hash << 11) ^ (*str++) ^ (hash >> 5)));
		}
	}
	return (hash&  0x7FFFFFFF);
}

//Initialization
xRedisAdmin::xRedisAdmin(RedisNode conn_list[], int conn_list_size)
{
enum {
CACHE_TYPE_1,
CACHE_TYPE_2,
CACHE_TYPE_MAX,
};

xRed.Init(CACHE_TYPE_MAX);
bool bret = xRed.ConnectRedisCache(conn_list, conn_list_size, CACHE_TYPE_1);
if (!bret) {
logging->error("Error connecting to Redis DB");
}
}

//Exists
bool xRedisAdmin::exists(const char * key)
{
  enum {
  CACHE_TYPE_1,
  CACHE_TYPE_2,
  CACHE_TYPE_MAX,
  };
RedisDBIdx d(&xRed);
d.CreateDBIndex(key, APHash, CACHE_TYPE_1);
char szKey[256] = {0};
sprintf(szKey, "%s", key);
return xRed.exists(d, szKey);
}

//Load
std::string xRedisAdmin::load(const char * key)
{
  enum {
  CACHE_TYPE_1,
  CACHE_TYPE_2,
  CACHE_TYPE_MAX,
  };
RedisDBIdx d(&xRed);
d.CreateDBIndex(key, APHash, CACHE_TYPE_1);
// char szKey[256] = {0};
// sprintf(szKey, "%s", key);
std::string strValue;
bool bret = xRed.get(d, key, strValue);
if (bret) {
return strValue;
}
else {
logging->error("Error Loading from Redis DB");
logging->error(d.GetErrInfo());
return "";
}
}

//Save
bool xRedisAdmin::save(const char * key, std::string val)
{
  enum {
  CACHE_TYPE_1,
  CACHE_TYPE_2,
  CACHE_TYPE_MAX,
  };
RedisDBIdx d(&xRed);
d.CreateDBIndex(key, APHash, CACHE_TYPE_1);
// char szKey[256] = {0};
// sprintf(szKey, "%s", key);
bool ret_val = xRed.set(d, key, val);
if (!ret_val) {
logging->error("Error writing to Redis DB");
logging->error(d.GetErrInfo());
}
return ret_val;
}

//Delete
bool xRedisAdmin::del(const char * key)
{
  enum {
  CACHE_TYPE_1,
  CACHE_TYPE_2,
  CACHE_TYPE_MAX,
  };
RedisDBIdx d(&xRed);
d.CreateDBIndex(key, APHash, CACHE_TYPE_1);
char szKey[256] = {0};
sprintf(szKey, "%s", key);
bool bret = xRed.del(d, szKey);
if (!bret) {
logging->error("Error Deleting from Redis DB");
logging->error(d.GetErrInfo());
}
return bret;
}

//Expire
bool xRedisAdmin::expire(const char * key, unsigned int second)
{
enum {
CACHE_TYPE_1,
CACHE_TYPE_2,
CACHE_TYPE_MAX,
};

RedisDBIdx d(&xRed);
d.CreateDBIndex(key, APHash, CACHE_TYPE_1);
char szKey[256] = {0};
sprintf(szKey, "%s", key);
bool bret = xRed.expire(d, key, second);
if (!bret) {
logging->error("Error setting expiration on Redis DB");
logging->error(d.GetErrInfo());
}
return bret;
}

//Close
xRedisAdmin::~xRedisAdmin()
{

}
