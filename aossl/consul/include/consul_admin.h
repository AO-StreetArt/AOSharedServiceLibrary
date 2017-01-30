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

#ifndef CONSUL_ADMIN
#define CONSUL_ADMIN

#include "aossl/http/client/include/http_admin.h"
#include "aossl/http/client/include/http_interface.h"
#include "aossl/http/client/include/factory_http_client.h"

#include "consul_interface.h"
#include "service.h"

#include <string>
#include <string.h>
#include <vector>

//------------------------------Consul Admin-----------------------------------//

//! The Consul Administrator, who handles distributed configuration & service discovery

//! This relies on the HTTP Administrator, and takes in a Service object in order to
//! register.  It's responses are JSON strings that are recieved from Consul.
//! Note that the values returned from the Key-Value store will be stored in base64 format
class ConsulAdmin: public ConsulInterface
{
  HttpInterface *ha = NULL;
  std::string consul_addr;
  int timeout;
  std::string build_url(std::string request_url_segment);
  std::string query(std::string query_url);
  static bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
  }
public:

  std::string base64_decode(std::string const& encoded_string);

  //! Construct a consul admin, passing in the connection string
  ConsulAdmin(std::string caddr) {HttpClientFactory http_factory; ha = http_factory.get_http_interface(); consul_addr = caddr;timeout=5;}

  //! Delete a consul admin
  ~ConsulAdmin() {delete ha;}

  //------------------Service Registry Functions------------------------------//

  //! Register the Service
  bool register_service(ServiceInterface& s);

  //! Deregister the Service
  bool deregister_service(ServiceInterface& s);

  //-------------Configuration Key-Value Storage Functions--------------------//

  //! Set a configuration value.

  //! If the key does not exist, then this will add it.
  //! Otherwise, it will update the existing key.
  bool set_config_value(std::string key, std::string val);

  //! Get a configuration value
  std::string get_config_value(std::string key);

  //! Delete a configuration value
  bool del_config_value(std::string key);

  //Basic Queries
  //All Return a JSON string

  //Local Agent Queries

  //! Query the local agent for services registered
  std::string services();

  //! Query the local agent for it's info
  std::string agent_info();

  //! Query for healthy services only
  std::string healthy_services();

  //Catalog Queries

  //! Query the catalog for datacenters
  std::string datacenters();

  //! Query the catalog for the nodes in a particular datacenter
  std::string nodes_dc(std::string data_center);

  //! Query the catalog for the services in a particular datacenter
  std::string services_dc(std::string data_center);

  //! Query the catalog for the nodes running a particular service
  std::string nodes_service(std::string service);

  //! Query the catalog for the services provided by a particular node
  std::string services_node(std::string node, std::string data_center);

};

#endif
