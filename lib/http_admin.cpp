#include "include/http_admin.h"

void HttpAdmin::bind_get_callback(WriteCallback new_func)
{
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, new_func);
}

bool HttpAdmin::send(char * url, int timeout)
{
  curl_easy_setopt(curl, CURLOPT_URL, url);
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
  return ret;
}

bool HttpAdmin::put(char * url, char * data, int timeout)
{
  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
  return send(url, timeout);
}

//Needs a write function registered
bool HttpAdmin::get(char * url, int timeout)
{
  return send(url, timeout);
}

bool HttpAdmin::post(char * url, char * data, int timeout)
{
  curl_easy_setopt(curl, CURLOPT_POST, 1L);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
  return send(url, timeout);
}

bool HttpAdmin::del(char * url, int timeout)
{
  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
  return send(url, timeout);
}
