//The HTTP Administrator
//Responsible for placing HTTP Requests using curl

#ifndef HTTP_ADMIN
#define HTTP_ADMIN

#include <string>
#include <sstream>
#include <curl/curl.h>

#include "factory/logging_interface.h"
#include "factory/http_interface.h"

//HTTP Callbacks
//A String to store response data
extern std::string writedata;

//This is the callback that gets called when we recieve the response to the
//Get Curl Request
size_t writeCallback(char * buf, size_t size, size_t nmemb, void* up);

//! The HTTP Requests Administrators

//! This class is in charge of making HTTP Requests
//! Support for put, get post, and delete
class HttpAdmin: public HttpInterface
{
CURL* curl;
std::string send(char * url, int timeout);
public:

  //! Start a new HTTP Requests Admin
  HttpAdmin() {curl_global_init(CURL_GLOBAL_ALL);curl = curl_easy_init();}

  //! Shutdown the admin
  void shutdown() {curl_global_cleanup();}

  ~HttpAdmin() {shutdown();}

  //! Return the instance to bind callbacks against.  Not advised
  CURL* get_instance() {return curl;}

  //HTTP Methods

  //! Put

  //! Put to the given URL the supplied data
  //! with the specified timeout
  std::string put(char * url, char * data, int timeout);

  //! Get

  //! Get from the given URL
  //! with the specified timeout
  std::string get(char * url, int timeout);

  //! Post

  //! Post to the given URL the supplied data
  //! with the specified timeout
  std::string post(char * url, char * data, int timeout);

  //! Delete

  //! Delete from the given URL
  //! with the specified timeout
  std::string del(char * url, int timeout);
};

#endif
