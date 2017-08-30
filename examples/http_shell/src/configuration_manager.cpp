/*
MIT License Block

Copyright (c) 2016 Alex Barry

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "include/configuration_manager.h"

ConfigurationManager::~ConfigurationManager() {
  ca->deregister_service(*s);
  if (consul_factory) delete consul_factory;
}

// --------------------------Configure from Consul----------------------------//

std::string ConfigurationManager::get_consul_config_value(std::string key) {
  std::string resp_str;
  // Get a JSON List of the responses
  std::string config_json = "";
  std::string query_key = "ivan/" + key;
  try {
    config_json = ca->get_config_value(query_key);
    config_logging->debug("Configuration JSON Retrieved:");
    config_logging->debug(config_json);
  }
  catch (std::exception& e) {
    config_logging->error("Exception during Consul Configuration Retrieval");
    config_logging->error(e.what());
    throw e;
  }

  // Parse the JSON Response
  rapidjson::Document d;

  if (!config_json.empty()) {
    try {
      config_logging->debug("Config Value retrieved from Consul:");
      config_logging->debug(key);
      config_logging->debug(config_json);
      const char * config_cstr = config_json.c_str();
      d.Parse(config_cstr);
    }
    // Catch a possible error and write to logs
    catch (std::exception& e) {
      config_logging->error("Exception while parsing Consul Service Response:");
      config_logging->error(e.what());
    }
  } else {
    config_logging->error("Configuration Value not found");
    config_logging->error(key);
    return resp_str;
  }

  // Get the object out of the array
  const rapidjson::Value& v = d[0];

  // if (v.IsObject())
  // {
    const rapidjson::Value& resp = v["Value"];
    if (resp.IsString()) {
      resp_str = resp.GetString();
      // Transform the object from base64
      return ca->base64_decode(resp_str);
    }
  // }
  return "";
}

// Configure based on the Services List and Key/Value store from Consul
bool ConfigurationManager::configure_from_consul(std::string consul_path, \
  std::string ip, std::string port) {
  // Now, use the Consul Admin to configure the app
  // Step 1a: Generate new connectivity information
  //             for the inbound service from command line arguments
  if (ip == "localhost") {
    HTTP_ip = "127.0.0.1";
  } else {
    HTTP_ip = ip;
  }

  // Step 1b: Register the Service with Consul

  // Build a new service definition for this currently running instance of ivan
  std::string name = "Adrestia";
  s = consul_factory->get_service_interface(node_id, name, HTTP_ip, port);
  s->add_tag("HTTP");

  // Register the service
  bool register_success = false;
  try {
    register_success = ca->register_service(*s);
  }
  catch (std::exception& e) {
    config_logging->error("Exception encountered during Service Registration");
    config_logging->error(e.what());
    throw e;
  }

  if (!register_success) {
    config_logging->error("Failed to register with Consul");
    return false;
  }

  return true;
}

// ---------------------External Configuration Methods------------------------//

// The publicly exposed function that determines where config comes from
bool ConfigurationManager::configure() {
  // Null Check
  if (!cli) {
    config_logging->error("Configure called with null CLI");
    return false;
  } else {
    bool ret_val = false;

    // See if we have any environment variables specified
    const char * env_consul_addr = std::getenv("ADRESTIA_CONSUL_CONN");
    const char * env_ip = std::getenv("ADRESTIA_IP");
    const char * env_port = std::getenv("ADRESTIA_PORT");

    // Check if we have a consul address specified
    if (env_consul_addr && env_ip && env_port) {
      std::string env_consul_addr_str(env_consul_addr);
      std::string env_ip_str(env_ip);
      std::string env_port_str(env_port);
      ret_val = \
        configure_from_consul(env_consul_addr_str, env_ip_str, env_port_str);
      if (!ret_val) {
        config_logging->error("Configuration from Consul failed");
      }
    } else if (cli->opt_exist("-consul") && \
      cli->opt_exist("-ip") && cli->opt_exist("-port")) {
      ret_val = configure_from_consul(cli->get_opt("-consul"), \
        cli->get_opt("-ip"), cli->get_opt("-port"));

      if (!ret_val) {
        config_logging->error("Configuration from Consul failed");
      }
    } else {
      config_logging->error("Insufficient inputs to start program");
    }

    return ret_val;
  }
  return false;
}
