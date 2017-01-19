#include "include/http_admin.h"

//Global writedata instantiation to store HTTP Callbacks
std::string writedata;

//This is the callback that gets called when we recieve the response to the
//Get Curl Request
size_t writeCallback(char * buf, size_t size, size_t nmemb, void* up)
{

//Put the response into a string
for (size_t c = 0; c<size*nmemb; c++)
{
	writedata.push_back(buf[c]);
}

return size*nmemb;
}

bool HttpAdmin::send(std::string url, int timeout)
{
  const char * url_cstr = url.c_str();
  char *url_s = new char[url.length() + 1];
  strcpy(url_s, url_cstr);

  curl_easy_setopt(curl, CURLOPT_URL, url_s);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
  CURLcode res = curl_easy_perform(curl);
  bool ret = false;
  if (CURLE_OK == res)
  {
  //Successful request performed
  ret = true;
  }
  else
  {
    std::ostringstream out;
    out << res;
    throw HttpRequestException(out.str());
  }

  curl_easy_cleanup(curl);
  curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
  delete url_s;
  return ret;
}

bool HttpAdmin::put(std::string url, std::string data, int timeout)
{
	std::lock_guard<std::mutex> lock(data_mutex);
  const char * data_cstr = data.c_str();
  char *data_s = new char[data.length() + 1];
  strcpy(data_s, data_cstr);

  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data_s);
  bool ret_val = send(url, timeout);
  delete data_s;
  return ret_val;
}

//Needs a write function registered
std::string HttpAdmin::get(std::string url, int timeout)
{
	std::lock_guard<std::mutex> lock(data_mutex);
  //Clear the string that will hold the response data.
  writedata.clear();
  send(url, timeout);
  return writedata;
}

bool HttpAdmin::post(std::string url, std::string data, int timeout)
{
	std::lock_guard<std::mutex> lock(data_mutex);
  const char * data_cstr = data.c_str();
  char *data_s = new char[data.length() + 1];
  strcpy(data_s, data_cstr);

  curl_easy_setopt(curl, CURLOPT_POST, 1L);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data_s);
  bool ret_val = send(url, timeout);
  delete data_s;
  return ret_val;
}

bool HttpAdmin::del(std::string url, int timeout)
{
	std::lock_guard<std::mutex> lock(data_mutex);
  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
  return send(url, timeout);
}
