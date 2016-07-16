#ifndef CONSUL_ADMIN
#define CONSUL_ADMIN

#include "http_admin.h"
#include "writeable.h"
#include "logging.h"

#include <string>
#include <string.h>
#include <vector>

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

//HTTP Callbacks
//A String to store response data
std::string writedata;

//This is the callback that gets called when we recieve the response to the
//Get Curl Request
size_t writeCallback(char * buf, size_t size, size_t nmemb, void* up);

//--------------------------------Service-------------------------------------//

//An instance of this class can be instantiated by a service and is
//passed to the consul admin to register and de-register
class Service
{
std::string id;
std::string name;
std::vector<std::string> tags;
std::string address;
std::string port;

public:
  //Constructors
  Service();
  Service(std::string new_id, std::string new_name);
  Service(std::string new_id, std::string new_name, std::string new_address, std::string new_port);
  Service(std::string new_id, std::string new_name, std::string new_address, std::string new_port, std::vector<std::string> new_tags);

  //Method that allows the service to be transformed into a json message to be sent via HTTP
  std::string to_json();

  //Get methods
  std::string get_id() {return id;}
  std::string get_name() {return name;}
  std::string get_address() {return address;}
  std::string get_port() {return port;}

  //Set methods
  void set_id(std::string new_id) {id = new_id;}
  void set_name(std::string new_name) {name = new_name;}
  void set_address(std::string new_address) {address = new_address;}
  void set_port(std::string new_port) {port = new_port;}

  //Get the tags
  std::vector<std::string> get_tags() {return tags;}
  //Add a tag
  void add_tag(std::string new_tag) {tags.push_back(new_tag);}
  //Clear the tags
	void clear_tags(){tags.clear();}
	//How many tags are there?
	int num_tags() const {return tags.size();}
};

//------------------------------Consul Admin-----------------------------------//

//The Consul Administrator, who handles distributed configuration & service discovery
class ConsulAdmin
{
HttpAdmin *ha;
std::string consul_addr;
int timeout;
std::string build_url(std::string request_url_segment);
std::string query(std::string query_url);
public:

  //Constructor & Destructor
  ConsulAdmin(std::string caddr) {ha = new HttpAdmin; consul_addr = caddr;timeout=5;}
  ~ConsulAdmin() {delete ha;}

  //------------------Service Registry Functions------------------------------//

  //Register & Deregister
  bool register_service(Service& s);
  bool deregister_service(Service& s);

  //Basic Queries
  //All Return a JSON string
  std::string datacenters();
  std::string nodes_dc(std::string data_center);
  std::string services_dc(std::string data_center);
  std::string nodes_service(std::string service);
  std::string services_node(std::string node, std::string data_center);

  //-------------Configuration Key-Value Storage Functions--------------------//
  bool set_config_value(std::string key, std::string val);
  std::string get_config_value(std::string key);
  bool del_config_value(std::string key);
};

#endif
