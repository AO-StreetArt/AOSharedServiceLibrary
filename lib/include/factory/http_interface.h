//The HTTP Administrator
//Responsible for placing HTTP Requests using curl

#include <string>

#ifndef HTTP_INTERFACE
#define HTTP_INTERFACE

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
  virtual std::string put(char * url, char * data, int timeout) = 0;

  //! Get

  //! Get from the given URL
  //! with the specified timeout
  virtual std::string get(char * url, int timeout) = 0;

  //! Post

  //! Post to the given URL the supplied data
  //! with the specified timeout
  virtual std::string post(char * url, char * data, int timeout) = 0;

  //! Delete

  //! Delete from the given URL
  //! with the specified timeout
  virtual std::string del(char * url, int timeout) = 0;
};

#endif
