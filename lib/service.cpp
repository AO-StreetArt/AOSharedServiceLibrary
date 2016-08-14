#include "include/service.h"

//--------------------------------Service-------------------------------------//

Service::Service()
{
  id="";
  name="";
  address="";
  port="";
  check.script = "";
}

Service::Service(std::string new_id, std::string new_name)
{
  id=new_id;
  name=new_name;
  address="";
  port="";
  check.script = "";
}

Service::Service(std::string new_id, std::string new_name, std::string new_address, std::string new_port)
{
  id=new_id;
  name=new_name;
  address=new_address;
  port=new_port;
  check.script = "";
}

Service::Service(std::string new_id, std::string new_name, std::string new_address, std::string new_port, std::vector<std::string> new_tags)
{
  id=new_id;
  name=new_name;
  address=new_address;
  port=new_port;
  tags=new_tags;
  check.script = "";
}

std::string Service::to_json()
{
  logging->debug("CONSUL: Service to JSON method Called:");
  logging->debug(id);

  std::string id_key = "ID";
  std::string name_key = "Name";
  std::string tags_key = "Tags";
  std::string addr_key = "Address";
  std::string port_key = "Port";

  //Build the base JSON String
  std::string json_str = "{\"" + id_key + "\": \"" + id + "\", \"" +
    name_key + "\": \"" + name + "\", \"";

  //Add in any service tags
  if (num_tags() > 0) {
    json_str = json_str + tags_key + "\": [\"" + tags[0] + "\"";

    for (int i=1; i<num_tags(); i++) {
      json_str = json_str + ", \"" + tags[i] + "\"";
    }

    json_str = json_str + "], \"";
  }

  //Finish the base json string
  json_str = json_str + addr_key + "\": \"" + address + "\"" +
    ", \"" + port_key + "\": " + port;

  //Add the Health Check
  if (!check.script.empty()) {
    json_str = json_str + ", \"check\": {\"script\": \"" + check.script + "\", \"interval\": \"" + check.interval + "\"}";
  }

  json_str = json_str + "}";

  logging->debug(json_str);

  return json_str;

}
