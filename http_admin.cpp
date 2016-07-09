#include "http_admin.h"

bool HttpAdmin::send(char * url, int timeout)
{
  logging->debug("HTTP: Sending HTTP Request");
  logging->debug(url);
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
  CURLcode res = curl_easy_perform(curl);
  bool ret = false;
  if (CURLE_OK == res)
  {
  //Successful request performed
  ret = true;
  logging->debug("HTTP: Sent HTTP Request");
  }
  else
  {
    std::ostringstream out;
    out << res;
    logging->error("HTTP: Error Sending HTTP Request");
    logging->error(out.str());
  }

  curl_easy_cleanup(curl);
  curl = curl_easy_init();
  return ret;
}

//Needs a Read function and pointer registered
bool HttpAdmin::put(char * url, int timeout)
{
  logging->debug("HTTP: Put Initiated");
  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
  return send(url, timeout);
}

//Needs a Read function and pointer registered
bool HttpAdmin::get(char * url, int timeout)
{
  logging->debug("HTTP: Get Initiated");
  return send(url, timeout);
}

//Needs a write function registered
bool HttpAdmin::post(char * url, char * data, int timeout)
{
  logging->debug("HTTP: Post Initiated with data:");
  logging->debug(data);
  curl_easy_setopt(curl, CURLOPT_POST, 1L);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
  return send(url, timeout);
}

bool HttpAdmin::del(char * url, int timeout)
{
  logging->debug("HTTP: Delete Initiated");
  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
  return send(url, timeout);
}
