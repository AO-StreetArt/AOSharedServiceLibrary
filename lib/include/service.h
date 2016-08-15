#include "factory/consul_interface.h"
#include <string>
#include <string.h>
#include <vector>

#ifndef AOSSL_SERVICE
#define AOSSL_SERVICE

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

#endif
