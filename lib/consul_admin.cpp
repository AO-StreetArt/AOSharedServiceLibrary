#include "include/consul_admin.h"

//Global writedata instantiation to store HTTP Callbacks
std::string writedata;

//----------------------------HTTP Callbacks----------------------------------//

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

//------------------------Consul Administrator--------------------------------//

std::string ConsulAdmin::base64_decode(std::string const& encoded_string) {

  static const std::string base64_chars =
               "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
               "abcdefghijklmnopqrstuvwxyz"
               "0123456789+/";

  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}

//Put together a url query segment with the consul address provided at initialization
std::string ConsulAdmin::build_url(std::string request_url_segment)
{
  std::string url = consul_addr;
  url = url + request_url_segment;
  return url;
}

//Post a query to consul
std::string ConsulAdmin::query(std::string query_url)
{
  //Clear the string that will hold the response data.
  writedata.clear();
  //Get the URL
  std::string url_string = build_url(query_url);
  const char * url_cstr = url_string.c_str();
  char *url = new char[url_string.length() + 1];
  strcpy(url, url_cstr);

  ha->bind_get_callback(writeCallback);

  //Send the HTTP Request
  bool success = ha->get(url, timeout);
  delete url;
  if (success)
  {
    return writedata;
  }
  else
  {
    return "";
  }
}

//-------------------Service Registry Functions-------------------------------//

bool ConsulAdmin::register_service(ServiceInterface& s)
{
  //Get the URL
  std::string url_string = build_url("/v1/agent/service/register");
  const char * url_cstr = url_string.c_str();
  char *url = new char[url_string.length() + 1];
  strcpy(url, url_cstr);

  //Get the message body
  std::string body_str = s.to_json();
  const char * body_cstr = body_str.c_str();
  char *body = new char[body_str.length() + 1];
  strcpy(body, body_cstr);

  //Send the HTTP Request
  bool success = ha->put(url, body, timeout);
  delete body;
  delete url;
  return success;
}

bool ConsulAdmin::deregister_service(ServiceInterface& s)
{
  //Get the URL
  std::string url_string = "/v1/agent/service/deregister/";
  url_string = url_string.append(s.get_id());
  url_string = build_url(url_string);
  const char * url_cstr = url_string.c_str();
  char *url = new char[url_string.length() + 1];
  strcpy(url, url_cstr);

  //Send the HTTP Request
  std::string empty_str = "";
  const char * empty_cstr = empty_str.c_str();
  char *empty_arr = new char[empty_str.length() + 1];
  strcpy(empty_arr, empty_cstr);
  bool success = ha->put(url, empty_arr, timeout);
  delete empty_arr;
  delete url;
  return success;
}

//----------------------------Basic Queries-----------------------------------//

std::string ConsulAdmin::services()
{
  std::string url = "/v1/agent/services";
  std::string result = query(url);
  return result;
}

std::string ConsulAdmin::agent_info()
{
  std::string url = "/v1/agent/self";
  std::string result = query(url);
  return result;
}

std::string ConsulAdmin::healthy_services()
{
  std::string url = "v1/health/service/web?passing";
  std::string result = query(url);
  return result;
}

//--------------Configuration Key-Value Storage Functions---------------------//

bool ConsulAdmin::set_config_value(std::string key, std::string val)
{
  //Get the URL
  std::string url_string = build_url("/v1/kv/");
  url_string = url_string.append(key);
  const char * url_cstr = url_string.c_str();
  char *url = new char[url_string.length() + 1];
  strcpy(url, url_cstr);

  //Get the message body
  const char * body_cstr = val.c_str();
  char *body = new char[val.length() + 1];
  strcpy(body, body_cstr);

  //Send the HTTP Request
  bool success = ha->put(url, body, timeout);
  delete body;
  delete url;
  return success;
}

std::string ConsulAdmin::get_config_value(std::string key)
{
  std::string url = "/v1/kv/";
  url = url.append(key);
  return query(url);
}

bool ConsulAdmin::del_config_value(std::string key)
{
  //Get the URL
  std::string url_string = build_url("/v1/kv/");
  url_string = url_string.append(key);
  const char * url_cstr = url_string.c_str();
  char *url = new char[url_string.length() + 1];
  strcpy(url, url_cstr);

  //Send the HTTP Request
  bool success = ha->del(url, timeout);
  delete url;
  return success;
}

std::string ConsulAdmin::datacenters()
{
  return query("/v1/catalog/datacenters");
}

std::string ConsulAdmin::nodes_dc(std::string data_center)
{
  std::string url = "/v1/catalog/nodes";
  if (!data_center.empty())
  {
    url = url.append("?dc=");
    url = url.append(data_center);
  }
  return query(url);
}

std::string ConsulAdmin::services_dc(std::string data_center)
{
  std::string url = "/v1/catalog/services";
  if (!data_center.empty())
  {
    url = url.append("?dc=");
    url = url.append(data_center);
  }
  return query(url);
}

std::string ConsulAdmin::nodes_service(std::string service)
{
  std::string url = "/v1/catalog/service/";
  url = url.append(service);
  return query(url);
}

std::string ConsulAdmin::services_node(std::string node, std::string data_center)
{
  std::string url = "/v1/catalog/node/";
  url = url.append(node);
  if (!data_center.empty())
  {
    url = url.append("?dc=");
    url = url.append(data_center);
  }
  return query(url);
}
