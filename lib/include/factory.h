#include "xredis/xRedisClient.h"
#include <string.h>
#include <zmq.hpp>

#include "factory/commandline_interface.h"
#include "factory/consul_interface.h"
#include "factory/couchbase_interface.h"
#include "factory/http_interface.h"
#include "factory/logging_interface.h"
#include "factory/redis_interface.h"
#include "factory/uuid_interface.h"
#include "factory/zmq_interface.h"

#include "service.h"
#include "cli.h"
#include "consul_admin.h"
#include "couchbase_admin.h"
#include "http_admin.h"
#include "logging.h"
#include "uuid_admin.h"
#include "xredis_admin.h"
#include "zmqio.h"

#ifndef AOSSL_FACTORY
#define AOSSL_FACTORY

//! The Service Component Factory

//! The Service Component Factory tracks the various
//! objects exposed by the framework and passes back
//! instances of interfaces.  This allows for the publicly exposed methods
//! to be independent of the implementations.
class ServiceComponentFactory
{
zmq::context_t *context;
public:

  //! Create a new Service Component Factory
  ServiceComponentFactory();

  //! Delete a Service Component Factory
  ~ServiceComponentFactory();

  //! Get the Command Line Interface instance
  CommandLineInterface* get_command_line_interface( int argc, char* argv[] );

  //! Get the UUID Interface instance
  uuidInterface* get_uuid_interface();

  //! Get the HTTP Interface instance
  HttpInterface* get_http_interface();

  //! Get a Consul Interface instance
  ConsulInterface* get_consul_interface( std::string caddr );

  //! Get a Service Interface instance
  ServiceInterface* get_service_interface();

  //! Get a Service Interface instance
  ServiceInterface* get_service_interface(std::string new_id, std::string new_name);

  //! Get a Service Interface instance
  ServiceInterface* get_service_interface(std::string new_id, std::string new_name, std::string new_address, std::string new_port);

  //! Get a Service Interface instance
  ServiceInterface* get_service_interface(std::string new_id, std::string new_name, std::string new_address, std::string new_port, std::vector<std::string> new_tags);

  //! Get a Couchbase Interface instance
  CouchbaseInterface* get_couchbase_interface( const char * conn );

  //! Get a Couchbase Interface instance for a password protected DB
  CouchbaseInterface* get_couchbase_interface( const char * conn, const char * pswd );

  //! Get a Redis Cluster Interface instance
  RedisInterface* get_redis_cluster_interface( std::vector<RedisConnChain> RedisConnectionList );

  //! Get a Logging Interface instance
  LoggingInterface* get_logging_interface( std::string initFileName );

  //! Get a ZMQ Outbound Interface instance
  Zmqio* get_zmq_outbound_interface( std::string conn_str );

  //! Get a ZMQ Inbound Interface instance
  Zmqio* get_zmq_inbound_interface( std::string conn_str );
};

#endif
