//This Interface Defines an object that is writeable to Couchbase

#ifndef WRITEABLE
#define WRITEABLE

class Writeable
{
public:
  virtual std::string get_key() = 0;
  virtual bool set_key(std::string new_key) = 0;
  //Convert the object to JSON
  virtual std::string to_json() = 0;
  //Convert the object to JSON Message
  virtual std::string to_json_msg(int msg_type) = 0;
  //Convert the object to a protocol buffer message
  virtual std::string to_protobuf_msg(int msg_type) = 0;
}

#endif
