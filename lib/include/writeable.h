//This Interface Defines an object that is writeable to Couchbase

#include <string>
#include <string.h>

#ifndef WRITEABLE
#define WRITEABLE

class Writeable
{
public:
  virtual std::string get_key() = 0;
  virtual bool set_key(std::string new_key) = 0;
  //Convert the object to JSON
  virtual std::string to_json() = 0;
};

#endif
