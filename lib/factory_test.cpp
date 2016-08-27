#include <iostream>
#include <string>

#include "include/factory.h"

#include "include/factory/commandline_interface.h"
#include "include/factory/consul_interface.h"
#include "include/factory/couchbase_interface.h"
#include "include/factory/http_interface.h"
#include "include/factory/logging_interface.h"
#include "include/factory/redis_interface.h"
#include "include/factory/uuid_interface.h"
#include "include/factory/zmq_interface.h"
#include "include/factory/http_server_interface.h"

int main( int argc, char** argv )
{

ServiceComponentFactory factory;

//! Get a Logging Interface instance
std::string initFileName = "test/log4cpp_test.properties";
logging = factory.get_logging_interface( initFileName );

CommandLineInterface *cli = factory.get_command_line_interface(argc, argv);

uuidInterface *uuid = factory.get_uuid_interface();

//! Get the HTTP Interface instance
HttpInterface *ha = factory.get_http_interface();

//! Get a Service Interface instance
ServiceInterface *s = factory.get_service_interface();

//! Get a Consul Interface instance
ConsulInterface *consul = factory.get_consul_interface( "localhost:8500" );

//! Get a Couchbase Interface instance
CouchbaseInterface *ca = factory.get_couchbase_interface( "couchbase://localhost/default" );

//! Get a Couchbase Interface instance for a password protected DB
//CouchbaseInterface *ca2 = factory.get_couchbase_interface( const char * conn, const char * pswd );

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

//! Get an HTTP Server Interface instance
HttpServerInterface *http = factory.get_http_server_interface("0.0.0.0", 12345);

//Run our tests

//Command Line Tests
std::cout << cli->get_program_name() << std::endl;
if ( cli->opt_exist("name") ) {
  std::cout << cli->get_opt("name") << std::endl;
}

delete cli;
delete uuid;
delete ha;
delete s;
delete consul;
delete ca;
delete ra;
delete zmqo;
delete zmqi;
delete logging;
delete http;

return 0;
}
