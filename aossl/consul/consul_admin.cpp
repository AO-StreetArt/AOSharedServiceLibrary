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

// Consul Administrator

AOSSL::ConsulAdmin::ConsulAdmin(std::string caddr) {
  AOSSL::HttpClientFactory http_factory;
  ha = http_factory.get_http_interface();
  consul_addr = caddr;
  timeout = 5;
}

void AOSSL::ConsulAdmin::base64_decode_by_reference(std::string const& encoded_string, AOSSL::StringBuffer& ret_buffer) {
  std::string decode_return_string = "";
  static const std::string base64_chars =
               "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
               "abcdefghijklmnopqrstuvwxyz"
               "0123456789+/";

  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];

  while (in_len-- && (encoded_string[in_] != '=') \
    && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = \
        ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        decode_return_string += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = \
      ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) decode_return_string += char_array_3[j];
  }
  ret_buffer.val.assign(decode_return_string);
  ret_buffer.success = true;
}

AOSSL::StringBuffer* \
  AOSSL::ConsulAdmin::base64_decode_safe(std::string const& encoded_string) {
  AOSSL::StringBuffer *buf = new AOSSL::StringBuffer;
  base64_decode_by_reference(encoded_string, *buf);
  return buf;
}

void AOSSL::ConsulAdmin::query_by_reference(std::string query_url, AOSSL::StringBuffer& ret_buffer) {
  // Get the URL
  std::string url_string = consul_addr;
  url_string = url_string + query_url;

  // Send the HTTP Request
  ret_buffer.val.assign(ha->get(url_string, timeout));
  if (ret_buffer.val.empty()) {
    ret_buffer.success = false;
    ret_buffer.err_msg.assign("No HTTP Response Recovered");
  } else {
    ret_buffer.success = true;
  }
}

// Post a query to consul
AOSSL::StringBuffer* AOSSL::ConsulAdmin::query_safe(std::string query_url) {
  // Build the request buffer
  AOSSL::StringBuffer *buf = new AOSSL::StringBuffer;
  query_by_reference(query_url, *buf);
  return buf;
}

// Service Registry Functions

bool AOSSL::ConsulAdmin::register_service(const ServiceInterface& s) {
  // Get the URL
  std::string query_url = "/v1/agent/service/register";
  std::string url_string = consul_addr + query_url;

  // Get the message body
  std::string body_str = s.to_json();

  // Send the HTTP Request
  bool success = ha->put(url_string, body_str, timeout);
  return success;
}

bool AOSSL::ConsulAdmin::deregister_service(const ServiceInterface& s) {
  // Get the URL
  std::string query_url = "/v1/agent/service/deregister/";
  query_url = query_url.append(s.get_id());
  std::string url_string = consul_addr + query_url;

  // Send the HTTP Request
  std::string empty_str = "";
  bool success = ha->put(url_string, empty_str, timeout);
  return success;
}

// Basic Queries

AOSSL::StringBuffer* AOSSL::ConsulAdmin::services() {
  std::string url = "/v1/agent/services";
  return query_safe(url);
}

AOSSL::StringBuffer* AOSSL::ConsulAdmin::agent_info() {
  std::string url = "/v1/agent/self";
  return query_safe(url);
}

AOSSL::StringBuffer* AOSSL::ConsulAdmin::healthy_services() {
  std::string url = "v1/health/service/web?passing";
  return query_safe(url);
}

// Configuration Key-Value Storage Functions

bool AOSSL::ConsulAdmin::set_config_value(std::string key, std::string val) {
  // Get the URL
  std::string query_url = "/v1/kv/";
  query_url = query_url.append(key);
  std::string url_string = consul_addr + query_url;

  // Send the HTTP Request
  bool success = ha->put(url_string, val, timeout);
  return success;
}

bool AOSSL::ConsulAdmin::del_config_value(std::string key) {
  // Get the URL
  std::string query_url = "/v1/kv/";
  query_url = query_url.append(key);
  std::string url_string = consul_addr + query_url;
  // Send the HTTP Request
  bool success = ha->del(url_string, timeout);
  return success;
}

AOSSL::StringBuffer* AOSSL::ConsulAdmin::datacenters() {
  return query_safe("/v1/catalog/datacenters");
}

AOSSL::StringBuffer* AOSSL::ConsulAdmin::nodes_dc(std::string data_center) {
  std::string url = "/v1/catalog/nodes";
  if (!data_center.empty()) {
    url = url.append("?dc=");
    url = url.append(data_center);
  }
  return query_safe(url);
}

AOSSL::StringBuffer* AOSSL::ConsulAdmin::services_dc(std::string data_center) {
  std::string url = "/v1/catalog/services";
  if (!data_center.empty()) {
    url = url.append("?dc=");
    url = url.append(data_center);
  }
  return query_safe(url);
}

AOSSL::StringBuffer* AOSSL::ConsulAdmin::nodes_service(std::string service) {
  std::string url = "/v1/catalog/service/";
  url = url.append(service);
  return query_safe(url);
}

AOSSL::StringBuffer* AOSSL::ConsulAdmin::services_node(std::string node, \
  std::string data_center) {
  std::string url = "/v1/catalog/node/";
  url = url.append(node);
  if (!data_center.empty()) {
    url = url.append("?dc=");
    url = url.append(data_center);
  }
  return query_safe(url);
}
