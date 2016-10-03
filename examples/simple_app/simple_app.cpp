#include <iostream>
#include <string>
#include <exception>
#include <signal.h>

#include "aossl/factory.h"

#include "aossl/factory/commandline_interface.h"
#include "aossl/factory/consul_interface.h"
#include "aossl/factory/couchbase_interface.h"
#include "aossl/factory/http_interface.h"
#include "aossl/factory/logging_interface.h"
#include "aossl/factory/redis_interface.h"
#include "aossl/factory/uuid_interface.h"
#include "aossl/factory/zmq_interface.h"

CommandLineInterface *cli;
uuidInterface *uuid;
HttpInterface *ha;
ServiceInterface *s;
ConsulInterface *consul;
CouchbaseInterface *ca;
RedisInterface *ra;
Zmqio *zmqo;
Zmqio *zmqi;

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
CommandLineInterpreterFactory cli_factory;
ConsulComponentFactory consul_factory;
CouchbaseComponentFactory couchbase_factory;
HttpClientFactory http_client_factory;
PropertyReaderFactory props_factory;
RedisComponentFactory redis_factory;
uuidComponentFactory uuid_factory;
ZmqComponentFactory zmq_factory;
LoggingComponentFactory logging_factory;

//Retrieve a new command line interface
cli = cli_factory.get_command_line_interface(argc, argv);

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
logging = logging_factory.get_logging_interface( initFileName );

// Get a UUID Generator
uuid = uuid_factory.get_uuid_interface();

//! Get the HTTP Interface instance
ha = http_client_factory.get_http_interface();

//! Get a Service Interface instance
s = consul_factory.get_service_interface();

//! Get a Consul Interface instance
consul = consul_factory.get_consul_interface( "localhost:8500" );

//! Get a Couchbase Interface instance
ca = couchbase_factory.get_couchbase_interface( "couchbase://localhost/default" );

//! Get a Redis Interface instance
ra = redis_factory.get_redis_interface( "127.0.0.1", 6379 );

//! Get a ZMQ Outbound Interface instance
zmqo = zmq_factory.get_zmq_outbound_interface( "tcp://localhost:5555" );

//! Get a ZMQ Inbound Interface instance
zmqi = zmq_factory.get_zmq_inbound_interface( "tcp://*:5555" );

//Here we have the core of the application
logging->info("Sending a message");

//Send a Message
std::string msg = "Test";
zmqo->send(msg);
bool keep_going = true;

while (keep_going) {

  //Convert the OMQ message into a string to be passed on the event
  std::string req_string = zmqi->recv();

  std::string resp = "success";
  logging->debug("Object Update Event Emitted, response:");
  logging->debug(resp);

  //  Send reply back to client
  zmqi->send(resp);
  logging->debug("Response Sent");

  keep_going = false;

}

std::string response = zmqo->recv();
logging->debug("Response Recieved");
logging->debug(response);

//Finally, we cleanup the app

shutdown();

return 0;
}
