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

#include <string.h>
#include <string>
#include <vector>
#include <iostream>

#include "aossl/core/include/base_http_client.h"
#include "aossl/core/include/buffers.h"

#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/SSLException.h"

#include "consul_interface.h"
#include "service.h"

#ifndef AOSSL_CONSUL_INCLUDE_CONSUL_ADMIN_H_
#define AOSSL_CONSUL_INCLUDE_CONSUL_ADMIN_H_

namespace AOSSL {

// Consul Admin

// The Consul Administrator, which handles distributed configuration
// & service discovery

// This relies on the HTTP Administrator, and takes in a Service object in
// order to register.  It's responses are JSON strings that are recieved
// from Consul.  Note that the values returned from the Key-Value store
// will be stored in base64 format
class ConsulAdmin: public BaseHttpClient, public ConsulInterface {
  inline static bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
  }
 public:
  // Base64 decoding function for accessing KV Store values
  void base64_decode_by_reference(std::string const& encoded_string, StringBuffer& ret_buffer);
  StringBuffer* base64_decode_safe(std::string const& encoded_string);

  // Add an ACL Token to the Consul Admin
  inline void add_acl_token(std::string& token) {
    BaseHttpClient::set_acl_token(std::string("X-Consul-Token"), token);
  }

  // Construct a consul admin, passing in the connection string
  ConsulAdmin(std::string& caddr) : BaseHttpClient(caddr, 5) {}

  // Construct a consul admin, passing in the connection string and timeout
  ConsulAdmin(std::string& caddr, int tout) : BaseHttpClient(caddr, tout) {}

  // Construct a consul admin, passing in the connection string and ssl cert
  ConsulAdmin(std::string& caddr, std::string& cert) : \
      BaseHttpClient(caddr, 5, cert) {}

  // Construct a consul admin, passing in the connection string, timeout, and ssl cert
  ConsulAdmin(std::string& caddr, int tout, std::string& cert) : \
      BaseHttpClient(caddr, tout, cert) {}

  // Construct a consul admin, passing in the connection string, timeout,
  // ssl cert, and consul acl token
  ConsulAdmin(std::string& caddr, int tout, std::string& cert, std::string& token) : \
      BaseHttpClient(caddr, tout, cert) {add_acl_token(token);}

  // Delete a consul admin
  ~ConsulAdmin() {}

  // Service Registry Functions

  // Register the Service
  bool register_service(const ServiceInterface& s);

  // Deregister the Service
  bool deregister_service(const ServiceInterface& s);

  // Configuration Key-Value Storage Functions

  // Set a configuration value.

  // If the key does not exist, then this will add it.
  // Otherwise, it will update the existing key.
  bool set_config_value(std::string key, std::string val);

  // Delete a configuration value
  bool del_config_value(std::string key);

  // Basic Queries
  // All Return a JSON string

  // Local Agent Queries

  // Query the local agent for services registered
  StringBuffer* services();

  // Query the local agent for it's info
  StringBuffer* agent_info();

  // Query for healthy services only
  StringBuffer* healthy_services();

  // Catalog Queries

  // Query the catalog for datacenters
  StringBuffer* datacenters();

  // Query the catalog for the nodes in a particular datacenter
  StringBuffer* nodes_dc(std::string data_center);

  // Query the catalog for the services in a particular datacenter
  StringBuffer* services_dc(std::string data_center);

  // Query the catalog for the nodes running a particular service
  StringBuffer* nodes_service(std::string service);

  // Query the catalog for the services provided by a particular node
  StringBuffer* \
    services_node(std::string node, std::string data_center);

  // Implementations of KeyValueStore interface
  //! Does a key exist?
  inline bool opt_exist(std::string key) {
    std::string url = "/v1/kv/";
    url = url.append(key);
    StringBuffer buf;
    BaseHttpClient::get_by_reference(url, buf);
    return buf.success;
  }

  //! Get an option by key
  inline StringBuffer* get_opt(std::string key) {
    std::string url = "/v1/kv/";
    url = url.append(key);
    StringBuffer *buf = new StringBuffer;
    BaseHttpClient::get_by_reference(url, *buf);
    return buf;
  }

  //! Get an option by key
  inline void get_opt(std::string key, StringBuffer& val) {
    std::string url = "/v1/kv/";
    url = url.append(key);
    BaseHttpClient::get_by_reference(url, val);
  }

  // Empty as getting any key always gets the most up-to-date
  // so we don't need to load our configuration repeatedly
  inline void load_config() {

  }
};

}

#endif  // AOSSL_CONSUL_INCLUDE_CONSUL_ADMIN_H_
