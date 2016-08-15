//The HTTP Administrator
//Responsible for placing HTTP Requests using curl

#ifndef HTTP_ADMIN
#define HTTP_ADMIN

#include <string>
#include <sstream>
#include <curl/curl.h>

#include "factory/logging_interface.h"
#include "factory/http_interface.h"

//! The HTTP Requests Administrators

//! This class is in charge of making HTTP Requests
//! Support for put, get post, and delete
class HttpAdmin: public HttpInterface
{
CURL* curl;
bool send(char * url, int timeout);
public:

  //! Start a new HTTP Requests Admin
  HttpAdmin() {curl_global_init(CURL_GLOBAL_ALL);curl = curl_easy_init();}

  //! Shutdown the admin
  void shutdown() {curl_global_cleanup();}

  ~HttpAdmin() {shutdown();}

  //! Bind Callback
  void bind_get_callback(WriteCallback);

  //! Return the instance to bind callbacks against.  Not advised
  CURL* get_instance() {return curl;}

  //HTTP Methods

  //! Put

  //! Put to the given URL the supplied data
  //! with the specified timeout
  bool put(char * url, char * data, int timeout);

  //! Get

  //! Get from the given URL
  //! with the specified timeout
  bool get(char * url, int timeout);

  //! Post

  //! Post to the given URL the supplied data
  //! with the specified timeout
  bool post(char * url, char * data, int timeout);

  //! Delete

  //! Delete from the given URL
  //! with the specified timeout
  bool del(char * url, int timeout);
};

#endif
