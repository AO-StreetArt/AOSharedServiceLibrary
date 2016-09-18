#include <string>
#include "writeable.h"

#ifndef APPLICATION_RESPONSE_INTERFACE
#define APPLICATION_RESPONSE_INTERFACE

const int NO_ERROR = 0;
const int ERROR = 100;
const int DB_ERROR = 110;
const int REDIS_ERROR = 120;
const int TRANSLATION_ERROR = 130;
const int BAD_SERVER_ERROR = 140;
const int BAD_REQUEST_ERROR = 150;

//! A class that gets sent to clients to transmit errors, transaction ID's, and object ID's
class ApplicationResponseInterface
{
public:

  virtual ~ApplicationResponseInterface() {}

  //! Get Error Code
  virtual int get_error_code() = 0;

  //! Get Error Code
  virtual std::string get_error_message() = 0;

  //! Get Error Code
  virtual std::string get_transaction_id() = 0;

  //! Get Error Code
  virtual std::string get_object_id() = 0;

  //! Set the transaction ID
  virtual void set_transaction_id(std::string new_tran_id) = 0;

  //! Set the object ID
  virtual void set_object_id(std::string new_obj_id) = 0;

  //! Set the error
  virtual void set_error(int new_error_code) = 0;

  //! Set the error
  virtual void set_error(int new_error_code, std::string new_error_message) = 0;

  //! To JSON Method
  virtual std::string to_json() = 0;

  //! To JSON Method which includes a writeable object
  virtual std::string to_json(Writeable *obj) = 0;

  //! Clear the response
  virtual void clear() = 0;
};

#endif
