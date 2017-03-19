/*
MIT License Block

Copyright (c) 2016 Alex Barry

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

//The HTTP Administrator
//Responsible for placing HTTP Requests using curl

#ifndef HTTP_ADMIN
#define HTTP_ADMIN

#include <string>
#include <string.h>
#include <sstream>
#include <curl/curl.h>
#include <mutex>

#include "http_interface.h"

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
  CURL* curl = NULL;
  std::mutex data_mutex;
  bool send(std::string url, int timeout);
public:

  //! Start a new HTTP Requests Admin
  HttpAdmin() {curl_global_init(CURL_GLOBAL_ALL);curl = curl_easy_init();curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);}

  //! Shutdown the admin
  void shutdown() {if (curl){curl_easy_cleanup(curl);} curl_global_cleanup();}

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
