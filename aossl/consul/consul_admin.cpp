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

#include "include/consul_admin.h"

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

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        base64_return_string += char_array_3[i];
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

    for (j = 0; (j < i - 1); j++) base64_return_string += char_array_3[j];
  }

  return base64_return_string;
}

//Post a query to consul
std::string ConsulAdmin::query(std::string query_url)
{
  //Get the URL
  std::string url_string = consul_addr;
  url_string = url_string + query_url;

  //Send the HTTP Request
  return_string = ha->get(url_string, timeout);
  if ( !(return_string.empty()) )
  {
    return return_string;
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
  std::string query_url = "/v1/agent/service/register";
  std::string url_string = consul_addr + query_url;

  //Get the message body
  std::string body_str = s.to_json();

  //Send the HTTP Request
  bool success = ha->put(url_string, body_str, timeout);
  return success;
}

bool ConsulAdmin::deregister_service(ServiceInterface& s)
{
  //Get the URL
  std::string query_url = "/v1/agent/service/deregister/";
  query_url = query_url.append(s.get_id());
  std::string url_string = consul_addr + query_url;

  //Send the HTTP Request
  std::string empty_str = "";
  bool success = ha->put(url_string, empty_str, timeout);
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
  std::string query_url = "/v1/kv/";
  query_url = query_url.append(key);
  std::string url_string = consul_addr + query_url;

  //Send the HTTP Request
  bool success = ha->put(url_string, val, timeout);
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
  std::string query_url = "/v1/kv/";
  query_url = query_url.append(key);
  std::string url_string = consul_addr + query_url;
  //Send the HTTP Request
  bool success = ha->del(url_string, timeout);
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
