#ifndef CONSUL_INTERFACE
#define CONSUL_INTERFACE

#include <string>
#include <string.h>
#include <vector>

//-----------------------------Health Checks----------------------------------//

//! A strcut to hold health check information which can be added to a service
struct HealthCheck
{
  std::string script;
  std::string interval;
};

//--------------------------------Service-------------------------------------//

//! A Service class which can be registered with Consul for each instance of a particular service

//! An instance of this class can be instantiated by a service and is
//! passed to the consul admin to register and de-register
class ServiceInterface
{
public:

  virtual ~ServiceInterface() = 0;
  //! Convert the Service into a JSON Message

  //! Method that allows the service to be transformed into a json message
  //! that can be sent via HTTP to a Consul instance
  virtual std::string to_json() = 0;

  //! Get the Service ID
  virtual std::string get_id() = 0;

  //! Get the Service Name
  virtual std::string get_name() = 0;

  //! Get the Service Address
  virtual std::string get_address() = 0;

  //! Get the Service Port
  virtual std::string get_port() = 0;

  //! Set the Service ID
  virtual void set_id(std::string new_id) = 0;

  //! Set the Service Name
  virtual void set_name(std::string new_name) = 0;

  //! Set the Service Address
  virtual void set_address(std::string new_address) = 0;

  //! Set the Service Port
  virtual void set_port(std::string new_port) = 0;

  //Get the tags
  virtual std::vector<std::string> get_tags() = 0;
  //Add a tag
  virtual void add_tag(std::string new_tag) = 0;
  //Clear the tags
	virtual void clear_tags() = 0;
	//How many tags are there?
	virtual int num_tags() const = 0;

  //Get the health checks
  virtual HealthCheck get_check() = 0;
  //Add a check
  virtual void set_check(std::string scr, int interval_seconds) = 0;
};

//------------------------------Consul Admin-----------------------------------//

//! The Consul Administrator, who handles distributed configuration & service discovery

//! This relies on the HTTP Administrator, and takes in a Service object in order to
//! register.  It's responses are JSON strings that are recieved from Consul.
//! Note that the values returned from the Key-Value store will be stored in base64 format
class ConsulInterface
{
public:

  virtual ~ConsulInterface() = 0;

  //------------------Service Registry Functions------------------------------//

  //! Register the Service
  virtual bool register_service(ServiceInterface& s) = 0;

  //! Deregister the Service
  virtual bool deregister_service(ServiceInterface& s) = 0;

  //-------------Configuration Key-Value Storage Functions--------------------//

  //! Set a configuration value.

  //! If the key does not exist, then this will add it.
  //! Otherwise, it will update the existing key.
  virtual bool set_config_value(std::string key, std::string val) = 0;

  //! Get a configuration value
  virtual std::string get_config_value(std::string key) = 0;

  //! Delete a configuration value
  virtual bool del_config_value(std::string key) = 0;

  //Basic Queries
  //All Return a JSON string

  //Local Agent Queries

  //! Query the local agent for services registered
  virtual std::string services() = 0;

  //! Query the local agent for it's info
  virtual std::string agent_info() = 0;

  //! Query for healthy services only
  virtual std::string healthy_services() = 0;

  //Catalog Queries

  //! Query the catalog for datacenters
  virtual std::string datacenters() = 0;

  //! Query the catalog for the nodes in a particular datacenter
  virtual std::string nodes_dc(std::string data_center) = 0;

  //! Query the catalog for the services in a particular datacenter
  virtual std::string services_dc(std::string data_center) = 0;

  //! Query the catalog for the nodes running a particular service
  virtual std::string nodes_service(std::string service) = 0;

  //! Query the catalog for the services provided by a particular node
  virtual std::string services_node(std::string node, std::string data_center) = 0;

};

#endif
