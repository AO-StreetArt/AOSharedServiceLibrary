#include "include/factory.h"

ServiceComponentFactory::ServiceComponentFactory()
{
  context = new zmq::context_t(1, 2);
}

ServiceComponentFactory::~ServiceComponentFactory()
{
  delete context;
}

//! Get the Command Line Interface instance
CommandLineInterface* ServiceComponentFactory::get_command_line_interface( int argc, char* argv[] )
{
  return new CommandLineInterpreter( argc, argv );
}

//! Get the UUID Interface instance
uuidInterface* ServiceComponentFactory::get_uuid_interface()
{
  return new uuidAdmin;
}

//! Get the HTTP Interface instance
HttpInterface* ServiceComponentFactory::get_http_interface()
{
  return new HttpAdmin;
}

ServiceInterface* ServiceComponentFactory::get_service_interface()
{
  return new Service;
}

//! Get a Service Interface instance
ServiceInterface* ServiceComponentFactory::get_service_interface(std::string new_id, std::string new_name)
{
  return new Service( new_id, new_name );
}

//! Get a Service Interface instance
ServiceInterface* ServiceComponentFactory::get_service_interface(std::string new_id, std::string new_name, std::string new_address, std::string new_port)
{
  return new Service( new_id, new_name, new_address, new_port );
}

//! Get a Service Interface instance
ServiceInterface* ServiceComponentFactory::get_service_interface(std::string new_id, std::string new_name, std::string new_address, std::string new_port, std::vector<std::string> new_tags)
{
  return new Service( new_id, new_name, new_address, new_port, new_tags );
}


//! Get a Consul Interface instance
ConsulInterface* ServiceComponentFactory::get_consul_interface( std::string caddr )
{
  return new ConsulAdmin( caddr );
}

//! Get a Couchbase Interface instance
CouchbaseInterface* ServiceComponentFactory::get_couchbase_interface( const char * conn )
{
  return new CouchbaseAdmin( conn );
}

//! Get a Couchbase Interface instance for a password protected DB
CouchbaseInterface* ServiceComponentFactory::get_couchbase_interface( const char * conn, const char * pswd )
{
  return new CouchbaseAdmin( conn, pswd );
}

//! Get a Redis Cluster Interface instance
RedisInterface* ServiceComponentFactory::get_redis_cluster_interface( std::vector<RedisConnChain> RedisConnectionList )
{
  //Set up the Redis Admin
  //Set up our Redis Connection List
  int conn_list_size = RedisConnectionList.size();
  RedisNode RedisList1[conn_list_size];
  for (int y = 0; y < conn_list_size; ++y)
  {
    //Pull the values from RedisConnectionList
    RedisNode redis_n;
    redis_n.dbindex = y;
    RedisConnChain redis_chain = RedisConnectionList[y];
    redis_n.host = redis_chain.ip.c_str();
    redis_n.port = redis_chain.port;
    redis_n.passwd = redis_chain.password.c_str();
    redis_n.poolsize = redis_chain.pool_size;
    redis_n.timeout = redis_chain.timeout;
    redis_n.role = redis_chain.role;
    logging->debug("Line added to Redis Configuration List with IP:");
    logging->debug(redis_n.host);

    RedisList1[y] = redis_n;
  }
  return new xRedisAdmin( RedisList1, conn_list_size );
}

//! Get a Logging Interface instance
LoggingInterface* ServiceComponentFactory::get_logging_interface( std::string initFileName )
{
  return new Logger( initFileName );
}

//! Get a ZMQ Outbound Interface instance
Zmqio* ServiceComponentFactory::get_zmq_outbound_interface( std::string conn_str )
{
  ZmqOut *zmqo =  new Zmqo( *context );
  zmqo->connect( conn_str );
  return zmqo;
}

//! Get a ZMQ Inbound Interface instance
Zmqio* ServiceComponentFactory::get_zmq_inbound_interface( std::string conn_str )
{
  ZmqIn *zmqi = new Zmqi( *context );
  zmqi->bind( conn_str );
  return zmqi;
}

HttpServerInterface* ServiceComponentFactory::get_http_server_interface()
{
  return new HttpServer();
}
