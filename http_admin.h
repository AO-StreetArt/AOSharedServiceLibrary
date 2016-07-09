//The HTTP Administrator
//Responsible for placing HTTP Requests using curl

#include <string>
#include <sstream>
#include <curl/curl.h>
#include "logging.h"

class HttpAdmin
{
CURL* curl;
bool send(char * url, int timeout);
public:
  //Initializer
  HttpAdmin() {curl_global_init(CURL_GLOBAL_ALL);curl = curl_easy_init();}
  //Shutdown the admin
  void shutdown() {curl_global_cleanup();}
  //Return the instance to bind callbacks against
  CURL* get_instance() {return curl;}
  //HTTP Methods
  bool put(char * url, int timeout);
  bool get(char * url, int timeout);
  bool post(char * url, char * data, int timeout);
  bool del(char * url, int timeout);
};
