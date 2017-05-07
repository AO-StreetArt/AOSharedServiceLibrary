//This implements the Configuration Manager

//This takes in a Command Line Interpreter, and based on the options provided,
//decides how the application needs to be configured.  It may configure either
//from a configuration file, or from a Consul agent

#ifndef CONFIG_MANAGER
#define CONFIG_MANAGER

const int PROTO_FORMAT = 0;
const int JSON_FORMAT = 1;

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
#include "aossl/redis/include/redis_interface.h"

#include "aossl/consul/include/factory_consul.h"
#include "aossl/properties/include/properties_reader_interface.h"
#include "aossl/properties/include/factory_props.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

class ConfigurationManager
{

ConsulComponentFactory *consul_factory = NULL;
PropertyReaderFactory *props_factory = NULL;

//Internal Consul Administrator
ConsulInterface *ca = NULL;
bool isConsulActive;

//Command Line Interpreter holding config arguments
CommandLineInterface *cli = NULL;

//Consul Service Definition
ServiceInterface *s = NULL;

//Configuration Variables
std::string DB_ConnStr;
std::string Mongo_ConnStr;
std::string Mongo_DbName;
std::string Mongo_DbCollection;
std::string OMQ_IBConnStr;
std::vector<RedisConnChain> RedisConnectionList;
int format_type;
bool StampTransactionId;
bool AtomicTransactions;

//The Current Node ID
std::string node_id;

//String Manipulations

//Split a string, based on python's split method
std::vector<std::string> split(std::string inp_string, char delim);

//Internal Configuration Methods

//File Config
//bool file_exists (std::string name);
bool configure_from_file (std::string file_path);

//Consul Config
std::string get_consul_config_value(std::string key);
bool configure_from_consul (std::string consul_path, std::string ip, std::string port);

public:
  //Constructor
  //Provides a set of default values
  ConfigurationManager(CommandLineInterface *c, std::string instance_id) {cli = c;\
    OMQ_IBConnStr="tcp://*:5555";Mongo_ConnStr="";Mongo_DbCollection="test";\
      isConsulActive=false;StampTransactionId=false;AtomicTransactions=false;\
        node_id=instance_id;consul_factory=new ConsulComponentFactory;\
          props_factory = new PropertyReaderFactory;format_type=-1;Mongo_DbName="test";}
  ~ConfigurationManager();

  //Populate the configuration variables
  bool configure();

  //Get configuration values
  std::string get_mongoconnstr() {return Mongo_ConnStr;}
  std::string get_dbname() {return Mongo_DbName;}
  std::string get_dbheadercollection() {return Mongo_DbCollection;}
  std::string get_ibconnstr() {return OMQ_IBConnStr;}
  std::vector<RedisConnChain> get_redisconnlist() {return RedisConnectionList;}
  bool get_transactionidsactive() {return StampTransactionId;}
  bool get_atomictransactions() {return AtomicTransactions;}
  int get_formattype() {return format_type;}

  //Get the Current Node ID
  std::string get_nodeid() {return node_id;}
};

#endif
