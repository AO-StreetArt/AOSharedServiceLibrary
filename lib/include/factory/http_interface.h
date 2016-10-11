//The HTTP Administrator
//Responsible for placing HTTP Requests using curl

#include <string>

#ifndef HTTP_INTERFACE
#define HTTP_INTERFACE

//! An Implementation of std::exception that denotes an error in Couchbase during bootstrap
struct HttpRequestException: public std::exception
{
  //! An error message passed on initialization
  std::string int_msg;

  HttpRequestException (std::string msg) {int_msg = msg;}
  HttpRequestException () {}
  ~HttpRequestException() throw () {}

  //! Show the error message in readable format
  const char * what() const throw ()
  {
    std::string what_str = "Error Sending HTTP Request: " + int_msg;
    return what_str.c_str();
  }
};

typedef size_t (*WriteCallback)(char *, size_t, size_t, void*);

//! The HTTP Requests Administrators

//! This class is in charge of making HTTP Requests
//! Support for put, get post, and delete
class HttpInterface
{
public:

  //! Shutdown the admin
  virtual void shutdown() = 0;

  virtual ~HttpInterface() {}

  //HTTP Methods

  //! Put

  //! Put to the given URL the supplied data
  //! with the specified timeout
  virtual bool put(std::string url, std::string data, int timeout) = 0;

  //! Get

  //! Get from the given URL
  //! with the specified timeout
  virtual std::string get(std::string url, int timeout) = 0;

  //! Post

  //! Post to the given URL the supplied data
  //! with the specified timeout
  virtual bool post(std::string url, std::string data, int timeout) = 0;

  //! Delete

  //! Delete from the given URL
  //! with the specified timeout
  virtual bool del(std::string url, int timeout) = 0;
};

#endif
