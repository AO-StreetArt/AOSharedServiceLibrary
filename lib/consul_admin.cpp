#include "consul_admin.h"

//----------------------------HTTP Callbacks----------------------------------//

//This is the callback that gets called when we recieve the response to the
//Get Curl Request
size_t writeCallback(char * buf, size_t size, size_t nmemb, void* up)
{

  logging->debug("CONSUL: HTTP Query Callback Triggered");

//Put the response into a string
for (int c = 0; c<size*nmemb; c++)
{
	writedata.push_back(buf[c]);
}

return size*nmemb;
}

//--------------------------------Service-------------------------------------//

Service::Service()
{
  id="";
  name="";
  address="";
  port="";
}

Service::Service(std::string new_id, std::string new_name)
{
  id=new_id;
  name=new_name;
  address="";
  port="";
}

Service::Service(std::string new_id, std::string new_name, std::string new_address, std::string new_port)
{
  id=new_id;
  name=new_name;
  address=new_address;
  port=new_port;
}

Service::Service(std::string new_id, std::string new_name, std::string new_address, std::string new_port, std::vector<std::string> new_tags)
{
  id=new_id;
  name=new_name;
  address=new_address;
  port=new_port;
  tags=new_tags;
}

std::string Service::to_register_json()
{
  logging->debug("CONSUL: Service to JSON method Called:");
  logging->debug(id);

  //Initialize the string buffer and writer
  rapidjson::StringBuffer s;
  rapidjson::Writer<rapidjson::StringBuffer> writer(s);

  //Start writing the object

  writer.StartObject();

  writer.Key("ID");
  std::string id = id;
  writer.String( id.c_str(), (SizeType)id.length() );

  writer.Key("Name");
  std::string name = name;
  writer.String( name.c_str(), (SizeType)name.length() );

  writer.Key("Tags");
  std::string tag;
  writer.StartArray();
  for (i=0; i<num_tags(); i++) {
    tag = tags[i];
    writer.String( tag.c_str(), (SizeType)tag.length() );
  }
  writer.EndArray();

  writer.Key("Address");
  std::string addr = address;
  writer.String( addr.c_str(), (SizeType)addr.length() );

  writer.Key("Port");
  std::string name = name;
  writer.String( name.c_str(), (SizeType)name.length() );

  writer.EndObject();

  //The Stringbuffer now contains a json message
  //of the object
	const char* ret_val = s.GetString();
	std::string ret_string (ret_val);
  logging->debug(ret_string);
  return ret_string;

}

//------------------------Consul Administrator--------------------------------//

//Put together a url query segment with the consul address provided at initialization
std::string ConsulAdmin::build_url(std::string request_url_segment)
{
  std::string url = consul_addr;
  url = url.append(request_url_segment);
  return url;
}

//Post a query to consul
std::string ConsulAdmin::query(std::string query_url)
{
  logging->info("CONSUL: Firing Query");
  logging->debug(query_url);
  //Clear the string that will hold the response data.
  writedata.clear()
  //Get the URL
  std::string url_string = build_url(query_url);
  const char * url_cstr = url_string.c_str();
  char *url = new char[url_string.length() + 1];
  strcpy(url, url_cstr);

  curl_easy_setopt(ha.get_instance(), CURLOPT_WRITEFUNCTION, &writeCallback);

  //Send the HTTP Request
  bool success = ha->get(url, timeout);
  delete url;
  if (success)
  {
    logging->debug("CONSUL: Query Successful");
    return writedata;
  }
  else
  {
    logging->debug("CONSUL: Query Failed");
    return "";
  }
}

//-------------------Service Registry Functions-------------------------------//

bool ConsulAdmin::register_service(Service& s)
{
  logging->debug("CONSUL: Registering Service");
  //Get the URL
  std::string url_string = build_url("/v1/agent/service/register");
  const char * url_cstr = url_string.c_str();
  char *url = new char[url_string.length() + 1];
  strcpy(url, url_cstr);

  //Get the message body
  std::string body_str = s.to_json();
  const char * body_cstr = body_str.c_str();
  char *body = new char[body_str.length() + 1];
  strcpy(body, body_cstr);

  //Send the HTTP Request
  bool success = ha->put(url, data, timeout);
  delete body;
  delete url;
  if (success)
  {
    logging->debug("CONSUL: Registration Successful");
  }
  else
  {
    logging->debug("CONSUL: Registration Unsuccessful");
  }
  return success;
}

bool ConsulAdmin::deregister_service(Service& s)
{
  //Get the URL
  std::string url_string = "/v1/agent/service/deregister/";
  url_string = url_string.append(id);
  url_string = build_url(url_string);
  const char * url_cstr = url_string.c_str();
  char *url = new char[url_string.length() + 1];
  strcpy(url, url_cstr);

  //Send the HTTP Request
  bool success = ha->put(url, "", timeout);
  delete url;
  if (success)
  {
    logging->debug("CONSUL: Deregistration Successful");
  }
  else
  {
    logging->debug("CONSUL: Deregistration Unsuccessful");
  }
  return success;
}

//----------------------------Basic Queries-----------------------------------//

std::string ConsulAdmin::datacenters()
{
  return query("/v1/catalog/datacenters");
}

std::string ConsulAdmin::nodes_dc(std::string data_center)
{
  std::string url = "/v1/catalog/nodes"
  if (!data_center.empty())
  {
    url = url.append("?dc=");
    url = url.append(data_center);
  }
  return query(url);
}

std::string ConsulAdmin::services_dc(std::string data_center)
{
  std::string url = "/v1/catalog/services"
  if (!data_center.empty())
  {
    url = url.append("?dc=");
    url = url.append(data_center);
  }
  return query(url);
}

std::string ConsulAdmin::nodes_service(std::string service)
{
  std::string url = "/v1/catalog/service/"
  url = url.append(service);
  return query(url);
}

std::string ConsulAdmin::services_node(std::string node, std::string data_center)
{
  std::string url = "/v1/catalog/node/"
  url = url.append(node);
  if (!data_center.empty())
  {
    url = url.append("?dc=");
    url = url.append(data_center);
  }
  return query(url);
}

//--------------Configuration Key-Value Storage Functions---------------------//

bool ConsulAdmin::set_config_value(std::string key, std::string val)
{
  logging->info("CONSUL: Setting Configuration Value");
  logging->debug(key);
  logging->debug(val);
  //Get the URL
  std::string url_string = build_url("/v1/kv/");
  url_string = url_string.append(key);
  const char * url_cstr = url_string.c_str();
  char *url = new char[url_string.length() + 1];
  strcpy(url, url_cstr);

  //Get the message body
  const char * body_cstr = val.c_str();
  char *body = new char[val.length() + 1];
  strcpy(body, body_cstr);

  //Send the HTTP Request
  bool success = ha->put(url, data, timeout);
  delete body;
  delete url;
  if (success)
  {
    logging->debug("CONSUL: Config Value Set Successful");
  }
  else
  {
    logging->debug("CONSUL: Config Value Set Unsuccessful");
  }
  return success;
}

std::string ConsulAdmin::get_config_value(std::string key)
{
  std::string url = "/v1/kv/"
  url = url.append(key);
  if (!data_center.empty())
  {
    url = url.append("?dc=");
    url = url.append(data_center);
  }
  return query(url);
}

bool ConsulAdmin::del_config_value(std::string key)
{
  logging->info("CONSUL: Deleting Configuration Value");
  logging->debug(key);
  //Get the URL
  std::string url_string = build_url("/v1/kv/");
  url_string = url_string.append(key);
  const char * url_cstr = url_string.c_str();
  char *url = new char[url_string.length() + 1];
  strcpy(url, url_cstr);

  //Send the HTTP Request
  bool success = ha->del(url, timeout);
  delete url;
  if (success)
  {
    logging->debug("CONSUL: Config Value Delete Successful");
  }
  else
  {
    logging->debug("CONSUL: Config Value Delete Unsuccessful");
  }
  return success;
}
