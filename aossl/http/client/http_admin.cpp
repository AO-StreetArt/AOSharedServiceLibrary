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

#include "include/http_admin.h"

// Global writedata instantiation to store HTTP Callbacks
std::string AOSSL::writedata;

bool AOSSL::HttpAdmin::send(std::string url, int timeout) {
  const char * url_cstr = url.c_str();
  char *url_s = new char[url.length() + 1];
  strcpy(url_s, url_cstr);

  curl_easy_setopt(curl, CURLOPT_URL, url_s);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
  CURLcode res = curl_easy_perform(curl);
  bool ret = false;
  if (CURLE_OK == res) {
    // Successful request performed
    ret = true;
  } else {
    std::ostringstream out;
    out << res;
    throw HttpRequestException(out.str());
  }

  curl_easy_reset(curl);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
  delete[] url_s;
  return ret;
}

bool AOSSL::HttpAdmin::put(std::string url, std::string data, int timeout) {
  std::lock_guard<std::mutex> lock(data_mutex);
  const char * data_cstr = data.c_str();
  char *data_s = new char[data.length() + 1];
  strcpy(data_s, data_cstr);

  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data_s);
  bool ret_val = send(url, timeout);
  delete[] data_s;
  return ret_val;
}

// Needs a write function registered
std::string AOSSL::HttpAdmin::get(std::string url, int timeout) {
  std::lock_guard<std::mutex> lock(data_mutex);
  // Clear the string that will hold the response data.
  writedata.clear();
  send(url, timeout);
  return writedata;
}

bool AOSSL::HttpAdmin::post(std::string url, std::string data, int timeout) {
  std::lock_guard<std::mutex> lock(data_mutex);
  const char * data_cstr = data.c_str();
  char *data_s = new char[data.length() + 1];
  strcpy(data_s, data_cstr);

  curl_easy_setopt(curl, CURLOPT_POST, 1L);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data_s);
  bool ret_val = send(url, timeout);
  delete[] data_s;
  return ret_val;
}

bool AOSSL::HttpAdmin::del(std::string url, int timeout) {
  std::lock_guard<std::mutex> lock(data_mutex);
  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
  return send(url, timeout);
}
