#include <string>

//This implements interfaces on callbacks in the form of function pointers,
//and structs that are passed to them on being called
//These function pointers are used as callbacks in both DB Admins and the HTTP Server

#ifndef CALLBACKS_INTERFACE
#define CALLBACKS_INTERFACE

//Constants defining types

const int UNKNOWN = 111;

const int HTTP_GET = 15;
const int HTTP_PUT = 16;
const int HTTP_POST = 17;
const int HTTP_DELETE = 18;

const int DB_CREATE = 19;
const int DB_SAVE = 20;
const int DB_GET = 21;
const int DB_DELETE = 22;

//Constants defining error codes
const int NOERROR = 0;
const int COUCHBASE_BADREQUEST = 801;

//! A struct that gets passed to callbacks to transmit errors
struct RequestError
{
  //! A numerical error code
  int err_code;

  //! An Error message
  std::string err_message;
  RequestError() {err_code = NOERROR; err_message = "";}
};

//! A struct that gets passed to callbacks
struct Request
{
  //! Used to store original request data
  std::string req_data;

  //! Used to store the address on the original request
  std::string req_addr;

  //! Used to store the request type
  int req_type;

  //! Used to store any error messages from the request
  RequestError *req_err;

  //! Used to store response data from the request
  std::string resp_data;

  //! Constructor
  Request() {req_data = ""; req_type = UNKNOWN; req_err = new RequestError(); resp_data = "";}
  ~Request() {delete req_err;}
};

typedef std::string (*CallbackInterface)(struct Request*);

#endif
