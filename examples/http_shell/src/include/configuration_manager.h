//This implements the Configuration Manager

//This takes in a Command Line Interpreter and Consul Agent, and based on the options provided,
//decides how the application needs to be configured.

#ifndef CONFIG_MANAGER
#define CONFIG_MANAGER

#include <string>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <errno.h>
#include <sys/stat.h>

#include "app_log.h"

#include "aossl/commandline/include/commandline_interface.h"
#include "aossl/consul/include/consul_interface.h"
#include "aossl/logging/include/logging_interface.h"

#include "aossl/consul/include/factory_consul.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

class ConfigurationManager
{

//Internal Consul Administrator
ConsulInterface *ca = NULL;

//Command Line Interpreter holding config arguments
CommandLineInterface *cli = NULL;

//Consul Service Definition
ServiceInterface *s = NULL;

ConsulComponentFactory *consul_factory = NULL;

//Configuration Variables
std::string HTTP_ip;
std::string HTTP_port;

//The Current Node ID
std::string node_id;

//Internal Configuration Methods

//Consul Config
std::string get_consul_config_value(std::string key);
bool configure_from_consul (std::string consul_path, std::string ip, std::string port);

public:
  //Constructor
  //Provides a set of default values that allow ex38 to run locally in a 'dev' mode
  ConfigurationManager(CommandLineInterface *c, ConsulInterface *cons, std::string instance_id) {cli = c;\
    ca = cons;node_id=instance_id;HTTP_ip="127.0.0.1";HTTP_port="12345";consul_factory = new ConsulComponentFactory;}
  ~ConfigurationManager();

  //Populate the configuration variables
  bool configure();

  //Get configuration values
  std::string get_ip() {return HTTP_ip;}
  std::string get_port() {return HTTP_port;}

  //Get the Current Node ID
  std::string get_nodeid() {return node_id;}
};

#endif
