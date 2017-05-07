#include <string>
#include <exception>
#include <cstdlib>
#include <stdlib.h>
#include <unordered_map>
#include "aossl/consul/include/consul_interface.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

struct ServiceConnection {
  std::string node_name;
  std::string ip;
  int port;
};

//Service Manager is responsible for serving up connections
class ServiceManager {
  ConsulInterface *consul = NULL;
  std::unordered_map<std::string, std::vector<ServiceConnection>> blacklist;
public:
  ServiceManager(ConsulInterface *c) {consul = c;}
  ~ServiceManager() {}

  //Get a Service Connection (IP & Port)
  inline ServiceConnection get_service_connection(std::string service_name) {

    unsigned int current_index = 0;

    //Set up the return variable
    ServiceConnection sc;
    sc.node_name = "";
    sc.ip = "";
    sc.port = -9999;

    //Retrieve a JSON list of service connection objects from Consul
    std::string services_json = consul->nodes_service(service_name);

    //Parse the JSON Response
    rapidjson::Document d;
    const char * conn_cstr = services_json.c_str();
    d.Parse(conn_cstr);

    bool service_found = false;

    while (service_found && (current_index < d.Size())) {

      //Get the correct values out of the first service in the translated JSON Document
      const rapidjson::Value& v = d[current_index];
      const rapidjson::Value& node_val = v["Node"];
      const rapidjson::Value& ip_val = v["ServiceAddress"];
      const rapidjson::Value& port_val = v["ServicePort"];

      //Pull the port and ip if available and not blacklisted
      if (node_val.IsString() && ip_val.IsString() && port_val.IsInt()) {
        sc.node_name = node_val.GetString();
        sc.ip = ip_val.GetString();
        sc.port = port_val.GetInt();
        service_found = false;
        std::vector<ServiceConnection> blist = blacklist[service_name];
        for (unsigned int i = 0; i < blist.size(); i++) {
          ServiceConnection bad_con = blist[i];
          if ((bad_con.node_name == sc.node_name) && (bad_con.ip == sc.ip) && (bad_con.port == sc.port)) {
            service_found = false;
            sc.node_name = "";
            sc.ip = "";
            sc.port = -9999;
          }
        }
      }
      current_index = current_index + 1;
    }
    return sc;
  }

  //Blacklist a service connection when no response is detected
  inline void blacklist_connection(std::string service_name, ServiceConnection sc) {
    auto search = blacklist.find(service_name);
    std::vector<ServiceConnection> sc_list;
    if (search != blacklist.end())
    {
      sc_list.push_back(sc);
      blacklist.emplace(service_name, sc_list);
    }
    else {
      sc_list = blacklist[service_name];
      sc_list.push_back(sc);
      blacklist[service_name] = sc_list;
    }
  }
};
