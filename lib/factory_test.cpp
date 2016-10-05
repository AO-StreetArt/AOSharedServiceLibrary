#include <iostream>
#include <string>

#include "include/factory_cli.h"
#include "include/factory_consul.h"
#include "include/factory_couchbase.h"
#include "include/factory_http_client.h"
#include "include/factory_http_server.h"
#include "include/factory_logging.h"
#include "include/factory_props.h"
#include "include/factory_redis.h"
#include "include/factory_uuid.h"
#include "include/factory_zmq.h"

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

CommandLineInterpreterFactory cli_factory;
ConsulComponentFactory consul_factory;
CouchbaseComponentFactory couchbase_factory;
HttpClientFactory http_client_factory;
HttpServerFactory http_server_factory;
PropertyReaderFactory props_factory;
RedisComponentFactory redis_factory;
uuidComponentFactory uuid_factory;
ZmqComponentFactory zmq_factory;
LoggingComponentFactory logging_factory;

//! Get a Logging Interface instance
std::string initFileName = "test/log4cpp_test.properties";
logging = logging_factory.get_logging_interface( initFileName );

CommandLineInterface *cli = cli_factory.get_command_line_interface(argc, argv);

uuidInterface *uuid = uuid_factory.get_uuid_interface();

//! Get the HTTP Interface instance
HttpInterface *ha = http_client_factory.get_http_interface();

//! Get a Service Interface instance
ServiceInterface *s = consul_factory.get_service_interface();

//! Get a Consul Interface instance
ConsulInterface *consul = consul_factory.get_consul_interface( "localhost:8500" );

//! Get a Couchbase Interface instance
CouchbaseInterface *ca = NULL;
try {
    ca = couchbase_factory.get_couchbase_interface( "couchbase://localhost/default" );
  }
catch (CouchbaseBootstrapException& be)
{
  std::cout << "Couchbase Bootstrap Exception" << std::endl;
  std::cout << be.what() << std::endl;
}
catch (CouchbaseConnectException& ce)
{
  std::cout << "Couchbase Bootstrap Exception" << std::endl;
  std::cout << ce.what() << std::endl;
}
//! Get a Couchbase Interface instance for a password protected DB
//CouchbaseInterface *ca2 = factory.get_couchbase_interface( const char * conn, const char * pswd );

//! Get a Redis Interface instance
RedisInterface *ra = redis_factory.get_redis_interface( "127.0.0.1", 6379 );

//! Get a ZMQ Outbound Interface instance
Zmqio *zmqo = zmq_factory.get_zmq_outbound_interface( "tcp://localhost:5555" );

//! Get a ZMQ Inbound Interface instance
Zmqio *zmqi = zmq_factory.get_zmq_inbound_interface( "tcp://*:5555" );

//! Get an HTTP Server Interface instance
HttpServerInterface *http = http_server_factory.get_http_server_interface("0.0.0.0", 12345);

//! Get a Properties File Reader Instance
PropertiesReaderInterface *props = props_factory.get_properties_reader_interface("test/test.properties");

//Run our tests

assert ( props );
assert ( http );
assert ( cli );
assert ( uuid );
assert ( ha );
assert ( s );
assert ( consul );
assert ( ca );
assert ( ra );
assert ( zmqo );
assert ( zmqi );
assert ( logging );

delete props;
delete http;
delete cli;
delete uuid;
delete ha;
delete s;
delete consul;
if (ca) {
  delete ca;
}
delete ra;
delete zmqo;
delete zmqi;
delete logging;

return 0;
}
