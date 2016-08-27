#include "include/http_admin.h"

//Global writedata instantiation to store HTTP Callbacks
std::string writedata;

//----------------------------HTTP Callbacks----------------------------------//

//This is the callback that gets called when we recieve the response to the
//Get Curl Request
size_t writeCallback(char * buf, size_t size, size_t nmemb, void* up)
{

  logging->debug("CONSUL: HTTP Query Callback Triggered");

//Put the response into a string
for (size_t c = 0; c<size*nmemb; c++)
{
	writedata.push_back(buf[c]);
}

return size*nmemb;
}

std::string HttpAdmin::send(char * url, int timeout)
{
  logging->debug("HTTP: Sending HTTP Request");
  logging->debug(url);
  writedata.clear();
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
  CURLcode res = curl_easy_perform(curl);
  if (CURLE_OK == res)
  {
  //Successful request performed
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
  return writedata;
}

std::string HttpAdmin::put(char * url, char * data, int timeout)
{
  logging->debug("HTTP: Put Initiated");
  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
  return send(url, timeout);
}

//Needs a write function registered
std::string HttpAdmin::get(char * url, int timeout)
{
  logging->debug("HTTP: Get Initiated");
  return send(url, timeout);
}

std::string HttpAdmin::post(char * url, char * data, int timeout)
{
  logging->debug("HTTP: Post Initiated with data:");
  logging->debug(data);
  curl_easy_setopt(curl, CURLOPT_POST, 1L);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
  return send(url, timeout);
}

std::string HttpAdmin::del(char * url, int timeout)
{
  logging->debug("HTTP: Delete Initiated");
  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
  return send(url, timeout);
}
