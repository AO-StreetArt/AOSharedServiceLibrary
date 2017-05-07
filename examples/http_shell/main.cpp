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

#include <string>
#include <exception>
#include <signal.h>
#include <cstdlib>
#include <stdlib.h>

#include "aossl/commandline/include/factory_cli.h"
#include "aossl/commandline/include/commandline_interface.h"

#include "aossl/uuid/include/uuid_interface.h"
#include "aossl/uuid/include/factory_uuid.h"

#include "aossl/zmq/include/zmq_interface.h"
#include "aossl/zmq/include/factory_zmq.h"

#include "aossl/consul/include/factory_consul.h"
#include "aossl/consul/include/consul_interface.h"

#include "aossl/http/server/include/factory_http_server.h"
#include "aossl/http/server/include/http_server_interface.h"

#include "aossl/logging/include/factory_logging.h"

#include "src/include/app_log.h"
#include "src/include/configuration_manager.h"
#include "src/include/consul_connector.h"

CommandLineInterface *cli = NULL;
uuidInterface *ua = NULL;
Zmqio *crazyivan_connection = NULL;
ConsulInterface *consul = NULL;
ConfigurationManager *config = NULL;
HttpServerInterface *http = NULL;
ServiceManager *services = NULL;

//Shutdown the application
void shutdown()
{
  //Delete objects off the heap
  if (crazyivan_connection) {
    delete crazyivan_connection;
  }
  if (ua) {
    delete ua;
  }
  if (config) {
    delete config;
  }
  if (services) {
    delete services;
  }
  if (consul) {
    delete cli;
  }
  if (cli) {
    delete cli;
  }
  if (http) {
    delete cli;
  }
  shutdown_logging_submodules();
  delete logging;
}

//Catch a Signal (for example, keyboard interrupt)
void my_signal_handler(int s){
   main_logging->error("Caught signal");
   std::string signal_type = std::to_string(s);
   main_logging->error(signal_type);
   shutdown_logging_submodules();
   shutdown();
   exit(1);
}

//Handle Ping Request
std::string process_ping(struct Request *req) {
  main_logging->debug("Processing Ping");
  std::string resp = "";
  UuidContainer id_container = ua->generate();
  if ( !(id_container.err.empty()) ) main_logging->error(id_container.err);
  std::string transaction_id = id_container.id;
  main_logging->info("Transaction ID: " + transaction_id);
  if (req->req_err->err_code == NOERROR)
	{
    resp = "{\"TransactionId\": \"" + transaction_id + "\", \"Status\": \"OK\"}";
  }
  else {
    resp = "{\"TransactionId\": \"" + transaction_id + "\", \"Error\": \"Request Error\"}";
    main_logging->error("Request Error");
    main_logging->error(req->req_err->err_message);
  }
  return resp;
}

//Handle base HTTP Requests
std::string process_request(struct Request *req)
{
  main_logging->debug("Processing Unbound Request");
  std::string resp = "";
  UuidContainer id_container = ua->generate();
  if ( !(id_container.err.empty()) ) main_logging->error(id_container.err);
  std::string transaction_id = id_container.id;
  main_logging->info("Transaction ID: " + transaction_id);

	if (req->req_err->err_code == NOERROR)
	{
    //TO-DO: Code in actual business logic
		if (req->req_type == HTTP_GET)
		{
			resp = "Get Request";
		}
		else if (req->req_type == HTTP_PUT)
		{
			resp = "Put Request";
		}
		else if (req->req_type == HTTP_POST)
		{
			resp = "Post Request";
		}
		else if (req->req_type == HTTP_DELETE)
		{
			resp = "Delete Request";
		}
		else
		{
			resp = "Unknown Request Type";
		}
	}
  else {
    resp = "{\"TransactionId\": \"" + transaction_id + "\", \"Error\": \"Request Error: " + std::to_string(req->req_err->err_code) + "\"}";
    main_logging->error("Request Error");
    main_logging->error(req->req_err->err_message);
  }
	return resp;
}

int main( int argc, char** argv )
{

  //Set up a handler for any signal events so that we always shutdown gracefully
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = my_signal_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;

  sigaction(SIGINT, &sigIntHandler, NULL);

  // Set up a Service Component Factory, where we get our application components
  CommandLineInterpreterFactory cli_factory;
  LoggingComponentFactory logging_factory;
  uuidComponentFactory uuid_factory;
  ZmqComponentFactory zmq_factory(3);
  HttpServerFactory server_factory;
  ConsulComponentFactory consul_factory;

  //Retrieve a new command line interface
  cli = cli_factory.get_command_line_interface(argc, argv);

  const char * env_log_conf = std::getenv("ADRESTIA_LOGGING_CONF");

  //We have an environment variable for the log config
  if (env_log_conf) {
    std::string initFileName (env_log_conf);
    logging = logging_factory.get_logging_interface( initFileName );
  }
  //See if we have a command line setting for the log file
  else if ( cli->opt_exist("-log-conf") ) {
    logging = logging_factory.get_logging_interface( cli->get_opt("-log-conf") );
  }
  //Default
  else
  {
    logging = logging_factory.get_logging_interface( "log4cpp_test.properties" );
  }

  //Set up the logging submodules for each category
  start_logging_submodules();

  const char * env_consul_conf = std::getenv("ADRESTIA_CONSUL_CONN");

  //Set up Consul Connection
  //We have an environment variable
  if (env_consul_conf) {
    std::string consul_path (env_consul_conf);
    consul = consul_factory.get_consul_interface( consul_path );
  }
  //See if we have a command line setting
  else if ( cli->opt_exist("-consul") ) {
    consul = consul_factory.get_consul_interface( cli->get_opt("-consul") );
  }
  //Default
  else
  {
    main_logging->error("Unable to establish Consul Connection due to lack of correct input");
    shutdown();
    exit(1);
  }

  //Set up the UUID Generator
  ua = uuid_factory.get_uuid_interface();
  main_logging->info("UUID Interface Initialized");

  //Get the instance ID
  std::string service_instance_id = "Adrestia-";
  try {
    UuidContainer id_container = ua->generate();
    if ( !(id_container.err.empty()) ) main_logging->error(id_container.err);
    service_instance_id = service_instance_id + id_container.id;
  }
  catch (std::exception& e) {
    main_logging->error("Exception encountered during Service Instance ID Generation");
    shutdown();
    exit(1);
  }

  //Set up the Configuration Manager
  config = new ConfigurationManager(cli, consul, service_instance_id);
  bool config_success = false;
  try {
    config_success = config->configure();
  }
  catch (std::exception& e) {
    main_logging->error("Exception encountered during Configuration");
    shutdown();
    exit(1);
  }
  if (!config_success)
  {
    main_logging->error("Configuration Failed");
    shutdown();
    exit(1);
  }

  services = new ServiceManager(consul);

  //Retrieve a connection to another service from Consul
  ServiceConnection ivan_service_conn = services->get_service_connection("Ivan");
  if (ivan_service_conn.ip == "" && ivan_service_conn.port == -9999) {
    main_logging->error("Unable to connect to running Crazy Ivan Instance");
  }
  else {
    crazyivan_connection = zmq_factory.get_zmq_outbound_interface(ivan_service_conn.ip, ivan_service_conn.port);
  }

  //Set up the HTTP Server
  int result;
  std::stringstream(config->get_port()) >> result;
  http = server_factory.get_http_server_interface(config->get_ip(), result);

  //Bind HTTP Callbacks
  http->bind_default_callback(process_request);
  http->bind_callback("/", process_ping);

  //Listen for Requests
  http->recv();

  //Finally, we cleanup the app
  shutdown();

  return 0;
}
