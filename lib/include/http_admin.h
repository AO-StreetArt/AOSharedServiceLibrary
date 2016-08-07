//The HTTP Administrator
//Responsible for placing HTTP Requests using curl

#ifndef HTTP_ADMIN
#define HTTP_ADMIN

#include <string>
#include <sstream>
#include <curl/curl.h>
#include "logging.h"

typedef size_t (*WriteCallback)(char *, size_t, size_t, void*);

class HttpAdmin
{
CURL* curl;
bool send(char * url, int timeout);
public:

  //Initializer
  HttpAdmin() {curl_global_init(CURL_GLOBAL_ALL);curl = curl_easy_init();}
  //Shutdown the admin
  void shutdown() {curl_global_cleanup();}
  //Bind Callbacks
  void bind_get_callback(WriteCallback);

  //Return the instance to bind callbacks against
  CURL* get_instance() {return curl;}

  //HTTP Methods
  bool put(char * url, char * data, int timeout);
  bool get(char * url, int timeout);
  bool post(char * url, char * data, int timeout);
  bool del(char * url, int timeout);
};

#endif
