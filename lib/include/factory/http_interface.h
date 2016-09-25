//The HTTP Administrator
//Responsible for placing HTTP Requests using curl

#include <string>

#ifndef HTTP_INTERFACE
#define HTTP_INTERFACE

struct HttpRequestException: public std::exception
{
  std::string int_msg;
  HttpRequestException (std::string msg) {int_msg = msg;}
  HttpRequestException () {}
  ~HttpRequestException() throw () {}
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

  //! Bind Callback
  virtual void bind_get_callback(WriteCallback) = 0;

  //HTTP Methods

  //! Put

  //! Put to the given URL the supplied data
  //! with the specified timeout
  virtual bool put(char * url, char * data, int timeout) = 0;

  //! Get

  //! Get from the given URL
  //! with the specified timeout
  virtual bool get(char * url, int timeout) = 0;

  //! Post

  //! Post to the given URL the supplied data
  //! with the specified timeout
  virtual bool post(char * url, char * data, int timeout) = 0;

  //! Delete

  //! Delete from the given URL
  //! with the specified timeout
  virtual bool del(char * url, int timeout) = 0;
};

#endif
