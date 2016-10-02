//The HTTP Administrator
//Responsible for placing HTTP Requests using curl

#ifndef HTTP_ADMIN
#define HTTP_ADMIN

#include <string>
#include <string.h>
#include <sstream>
#include <curl/curl.h>

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
bool send(std::string url, int timeout);
public:

  //! Start a new HTTP Requests Admin
  HttpAdmin() {curl_global_init(CURL_GLOBAL_ALL);curl = curl_easy_init();curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);}

  //! Shutdown the admin
  void shutdown() {curl_global_cleanup();}

  ~HttpAdmin() {shutdown();}

  //! Return the instance to bind callbacks against.  Not advised
  CURL* get_instance() {return curl;}

  //HTTP Methods

  //! Put

  //! Put to the given URL the supplied data
  //! with the specified timeout
  bool put(std::string url, std::string data, int timeout);

  //! Get

  //! Get from the given URL
  //! with the specified timeout
  std::string get(std::string url, int timeout);

  //! Post

  //! Post to the given URL the supplied data
  //! with the specified timeout
  bool post(std::string url, std::string data, int timeout);

  //! Delete

  //! Delete from the given URL
  //! with the specified timeout
  bool del(std::string url, int timeout);
};

#endif
