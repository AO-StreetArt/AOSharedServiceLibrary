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

// This implements the Configuration Manager

// This takes in a Command Line Interpreter and Consul Agent, and based on
// the options provided, decides how the application needs to be configured.

#ifndef EXAMPLES_HTTP_SHELL_SRC_INCLUDE_CONFIGURATION_MANAGER_H_
#define EXAMPLES_HTTP_SHELL_SRC_INCLUDE_CONFIGURATION_MANAGER_H_

#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "app_log.h"

#include "aossl/commandline/include/commandline_interface.h"
#include "aossl/consul/include/consul_interface.h"
#include "aossl/logging/include/logging_interface.h"

#include "aossl/consul/include/factory_consul.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

class ConfigurationManager {
// Internal Consul Administrator
ConsulInterface *ca = NULL;

// Command Line Interpreter holding config arguments
CommandLineInterface *cli = NULL;

// Consul Service Definition
ServiceInterface *s = NULL;

ConsulComponentFactory *consul_factory = NULL;

// Configuration Variables
std::string HTTP_ip;
std::string HTTP_port;

// The Current Node ID
std::string node_id;

// Internal Configuration Methods

// Consul Config
std::string get_consul_config_value(std::string key);
bool configure_from_consul(std::string consul_path, \
  std::string ip, std::string port);

 public:
  // Constructor
  // Provides a set of default values
  ConfigurationManager(CommandLineInterface *c, \
    ConsulInterface *cons, std::string instance_id) {
    // Update Internal Variables to those supplied
    cli = c;
    ca = cons;
    node_id = instance_id;
    // Set default values
    HTTP_ip = "127.0.0.1";
    HTTP_port = "12345";
    consul_factory = new ConsulComponentFactory;
  }

  // Destructor
  ~ConfigurationManager();

  // Populate the configuration variables
  bool configure();

  // Get configuration values
  std::string get_ip() {return HTTP_ip;}
  std::string get_port() {return HTTP_port;}

  // Get the Current Node ID
  std::string get_nodeid() {return node_id;}
};

#endif  // EXAMPLES_HTTP_SHELL_SRC_INCLUDE_CONFIGURATION_MANAGER_H_
