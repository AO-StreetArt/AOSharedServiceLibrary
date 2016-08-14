#include <iostream>
#include <string>

#include "aossl/factory.h"

#include "aossl/factory/commandline_interface.h"
#include "aossl/factory/consul_interface.h"
#include "aossl/factory/couchbase_interface.h"
#include "aossl/factory/http_interface.h"
#include "aossl/factory/logging_interface.h"
#include "aossl/factory/redis_interface.h"
#include "aossl/factory/uuid_interface.h"
#include "aossl/factory/zmq_interface.h"

//Shutdown the application
void shutdown()
{
  //Delete objects off the heap
  delete cli;
  delete uuid;
  delete ha;
  delete s;
  delete consul;
  delete ca;
  delete ra;
  delete logging;
  delete zmqo;
  delete zmqi;
}

//Catch a Signal (for example, keyboard interrupt)
void my_signal_handler(int s){
   logging->error("Caught signal");
   std::string signal_type = std::to_string(s);
   logging->error(signal_type);
   shutdown();
   exit(1);
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
ServiceComponentFactory factory;

//Retrieve a new command line interface
CommandLineInterface *cli = factory.get_command_line_interface(argc, argv);

std::string initFileName;

//See if we have a command line setting for the log file
if ( cli->opt_exist("-log-conf") ) {
  initFileName = cli->get_opt("-log-conf");
}
else
{
  initFileName = "log4cpp_test.properties";
}

//! Get a Logging Interface instance, and pass to the global logging instance
logging = factory.get_logging_interface( initFileName );

// Get a UUID Generator
uuidInterface *uuid = factory.get_uuid_interface();

//! Get the HTTP Interface instance
HttpInterface *ha = factory.get_http_interface();

//! Get a Service Interface instance
ServiceInterface *s = factory.get_service_interface();

//! Get a Consul Interface instance
ConsulInterface *consul = factory.get_consul_interface( "localhost:8500" );

//! Get a Couchbase Interface instance
CouchbaseInterface *ca = factory.get_couchbase_interface( "couchbase://localhost/default" );

//! Get a Redis Cluster Interface instance
std::vector<RedisConnChain> RedisConnectionList;
RedisConnChain r;
r.ip = "127.0.0.1";
r.port = 6379;
r.password = "";
r.pool_size = 2;
r.timeout = 5;
r.role = 0;
RedisConnectionList.push_back(r);
RedisInterface *ra = factory.get_redis_cluster_interface( RedisConnectionList );

//! Get a ZMQ Outbound Interface instance
Zmqio *zmqo = factory.get_zmq_outbound_interface( "tcp://localhost:5555" );

//! Get a ZMQ Inbound Interface instance
Zmqio *zmqi = factory.get_zmq_inbound_interface( "tcp://*:5555" );

//Here we have the core of the application
logging->error("Hello world!");

//Finally, we cleanup the app

shutdown();

return 0;
}
