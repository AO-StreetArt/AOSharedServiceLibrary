#include "include/consul_admin.h"

//Global writedata instantiation to store HTTP Callbacks
std::string writedata;

//----------------------------HTTP Callbacks----------------------------------//

//This is the callback that gets called when we recieve the response to the
//Get Curl Request
size_t writeCallback(char * buf, size_t size, size_t nmemb, void* up)
{

  logging->debug("CONSUL: HTTP Query Callback Triggered");

//Put the response into a string
for (size_t c = 0; c<size*nmemb; c++)
{
	writedata.push_back(buf[c]);
}

return size*nmemb;
}

//------------------------Consul Administrator--------------------------------//

//Put together a url query segment with the consul address provided at initialization
std::string ConsulAdmin::build_url(std::string request_url_segment)
{
  std::string url = consul_addr;
  url = url + request_url_segment;
  return url;
}

//Post a query to consul
std::string ConsulAdmin::query(std::string query_url)
{
  logging->info("CONSUL: Firing Query");
  logging->debug(query_url);
  //Clear the string that will hold the response data.
  writedata.clear();
  //Get the URL
  std::string url_string = build_url(query_url);
  const char * url_cstr = url_string.c_str();
  char *url = new char[url_string.length() + 1];
  strcpy(url, url_cstr);

  ha->bind_get_callback(writeCallback);

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

bool ConsulAdmin::register_service(ServiceInterface& s)
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
  bool success = ha->put(url, body, timeout);
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

bool ConsulAdmin::deregister_service(ServiceInterface& s)
{
  //Get the URL
  std::string url_string = "/v1/agent/service/deregister/";
  url_string = url_string.append(s.get_id());
  url_string = build_url(url_string);
  const char * url_cstr = url_string.c_str();
  char *url = new char[url_string.length() + 1];
  strcpy(url, url_cstr);

  //Send the HTTP Request
  std::string empty_str = "";
  const char * empty_cstr = empty_str.c_str();
  char *empty_arr = new char[empty_str.length() + 1];
  strcpy(empty_arr, empty_cstr);
  bool success = ha->put(url, empty_arr, timeout);
  delete empty_arr;
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

std::string ConsulAdmin::services()
{
  std::string url = "/v1/agent/services";
  std::string result = query(url);
  return result;
}

std::string ConsulAdmin::agent_info()
{
  std::string url = "/v1/agent/self";
  std::string result = query(url);
  return result;
}

std::string ConsulAdmin::healthy_services()
{
  std::string url = "v1/health/service/web?passing";
  std::string result = query(url);
  return result;
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
  bool success = ha->put(url, body, timeout);
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
  std::string url = "/v1/kv/";
  url = url.append(key);
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

std::string ConsulAdmin::datacenters()
{
  return query("/v1/catalog/datacenters");
}

std::string ConsulAdmin::nodes_dc(std::string data_center)
{
  std::string url = "/v1/catalog/nodes";
  if (!data_center.empty())
  {
    url = url.append("?dc=");
    url = url.append(data_center);
  }
  return query(url);
}

std::string ConsulAdmin::services_dc(std::string data_center)
{
  std::string url = "/v1/catalog/services";
  if (!data_center.empty())
  {
    url = url.append("?dc=");
    url = url.append(data_center);
  }
  return query(url);
}

std::string ConsulAdmin::nodes_service(std::string service)
{
  std::string url = "/v1/catalog/service/";
  url = url.append(service);
  return query(url);
}

std::string ConsulAdmin::services_node(std::string node, std::string data_center)
{
  std::string url = "/v1/catalog/node/";
  url = url.append(node);
  if (!data_center.empty())
  {
    url = url.append("?dc=");
    url = url.append(data_center);
  }
  return query(url);
}
