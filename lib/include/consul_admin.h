#ifndef CONSUL_ADMIN
#define CONSUL_ADMIN

#include "http_admin.h"
#include "factory/logging_interface.h"

#include "factory/consul_interface.h"
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
HttpAdmin *ha;
std::string consul_addr;
int timeout;
std::string build_url(std::string request_url_segment);
std::string query(std::string query_url);
public:

  //! Construct a consul admin, passing in the connection string
  ConsulAdmin(std::string caddr) {ha = new HttpAdmin; consul_addr = caddr;timeout=5;}

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
