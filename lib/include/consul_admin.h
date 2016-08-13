#ifndef CONSUL_ADMIN
#define CONSUL_ADMIN

#include "http_admin.h"
#include "factory/logging_interface.h"

#include "factory/consul_interface.h"

#include <string>
#include <string.h>
#include <vector>

//HTTP Callbacks
//A String to store response data
extern std::string writedata;

//This is the callback that gets called when we recieve the response to the
//Get Curl Request
size_t writeCallback(char * buf, size_t size, size_t nmemb, void* up);

//--------------------------------Service-------------------------------------//

//! A Service class which can be registered with Consul for each instance of a particular service

//! An instance of this class can be instantiated by a service and is
//! passed to the consul admin to register and de-register
class Service: public ServiceInterface
{
std::string id;
std::string name;
std::vector<std::string> tags;
std::string address;
std::string port;
HealthCheck check;

public:
  //! Construct a Service
  Service();

  //! Construct a Service
  Service(std::string new_id, std::string new_name);

  //! Construct a Service
  Service(std::string new_id, std::string new_name, std::string new_address, std::string new_port);

  //! Construct a Service
  Service(std::string new_id, std::string new_name, std::string new_address, std::string new_port, std::vector<std::string> new_tags);

  //! Convert the Service into a JSON Message

  //! Method that allows the service to be transformed into a json message
  //! that can be sent via HTTP to a Consul instance
  std::string to_json();

  //! Get the Service ID
  std::string get_id() {return id;}

  //! Get the Service Name
  std::string get_name() {return name;}

  //! Get the Service Address
  std::string get_address() {return address;}

  //! Get the Service Port
  std::string get_port() {return port;}

  //! Set the Service ID
  void set_id(std::string new_id) {id = new_id;}

  //! Set the Service Name
  void set_name(std::string new_name) {name = new_name;}

  //! Set the Service Address
  void set_address(std::string new_address) {address = new_address;}

  //! Set the Service Port
  void set_port(std::string new_port) {port = new_port;}

  //Get the tags
  std::vector<std::string> get_tags() {return tags;}
  //Add a tag
  void add_tag(std::string new_tag) {tags.push_back(new_tag);}
  //Clear the tags
	void clear_tags(){tags.clear();}
	//How many tags are there?
	int num_tags() const {return tags.size();}

  //Get the health checks
  HealthCheck get_check() {return check;}
  //Add a check
  void set_check(std::string scr, int interval_seconds) {check.script = scr; check.interval = std::to_string(interval_seconds) + "s";}
};

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
  bool register_service(Service& s);

  //! Deregister the Service
  bool deregister_service(Service& s);

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
