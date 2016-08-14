//This Interface Defines an object that is writeable to Couchbase

#include <string>
#include <string.h>

#ifndef WRITEABLE
#define WRITEABLE

//! The Writeable Interface

//! The Writeable interface is used by the DB Administrators
//! In order to define a class of objects that can be written to a JSON Document,
//! and therefore sent to a Document-based Database
class Writeable
{
public:

  virtual ~Writeable() = 0;

  //! Get the Object Key
  virtual std::string get_key() = 0;

  //! Set the Object Key
  virtual bool set_key(std::string new_key) = 0;

  //! Convert the object to JSON
  virtual std::string to_json() = 0;
};

#endif
