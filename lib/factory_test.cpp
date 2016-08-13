#include <iostream>
#include <string>

#include "lib/include/factory.h"

#include "lib/include/factory/commandline_interface.h"
#include "lib/include/factory/consul_interface.h"
#include "lib/include/factory/couchbase_interface.h"
#include "lib/include/factory/http_interface.h"
#include "lib/include/factory/logging_interface.h"
#include "lib/include/factory/redis_interface.h"
#include "lib/include/factory/uuid_interface.h"
#include "lib/include/factory/zmq_interface.h"

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
r.port = 7000;
r.password = "";
r.pool_size = 2;
r.timeout = 5;
r.role = 0;
RedisConnectionList.push_back(r);
RedisInterface *ra = factory.get_redis_cluster_interface( RedisConnectionList );

//! Get a ZMQ Outbound Interface instance
ZmqOut *zmqo = factory.get_zmq_outbound_interface( "tcp://localhost:5555" );

//! Get a ZMQ Inbound Interface instance
ZmqIn *zmqi = factory.get_zmq_inbound_interface( "tcp://*:5555" )

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
delete ca2;
delete ra;
delete logging;
delete zmqo;
delete zmqi;

return 0;
}
