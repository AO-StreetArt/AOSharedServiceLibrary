//tests for the configuration manager
//src/test/test.properties

#include "configuration_manager.h"
#include <assert.h>

#include "aossl/commandline/include/commandline_interface.h"
#include "aossl/commandline/include/factory_cli.h"

#include "aossl/logging/include/logging_interface.h"
#include "aossl/logging/include/factory_logging.h"

#include "aossl/uuid/include/uuid_interface.h"
#include "aossl/uuid/include/factory_uuid.h"

int main( int argc, char** argv )
{

  LoggingComponentFactory *logging_factory = new LoggingComponentFactory;
  CommandLineInterpreterFactory *cli_factory = new CommandLineInterpreterFactory;
  uuidComponentFactory *id_factory = new uuidComponentFactory;

  //-------------------------------Logging--------------------------------------//
  //----------------------------------------------------------------------------//

  std::string initFileName = "tests/log4cpp.properties";
  logging = logging_factory->get_logging_interface(initFileName);

  start_logging_submodules();

  logging->debug("PreTest Setup");

  //Set up the UUID Generator
  uuidInterface *ua = id_factory->get_uuid_interface();

  //Set up our command line interpreter
  CommandLineInterface *cli = cli_factory->get_command_line_interface( argc, argv );

  UuidContainer id_container = ua->generate();
  if (!id_container.err.empty()) {
    main_logging->error(id_container.err);
  }

  ConfigurationManager cm( cli, id_container.id );

  logging->debug("Configure the app");

  cm.configure();

  logging->debug("Checking Variable Retrieval");

  //Basic Tests

  assert ( cm.get_dbconnstr() == "couchbase://localhost/default" );
  assert ( cm.get_ibconnstr() == "tcp://*:5555" );
  logging->debug("Values checked");

  //Redis Connection List Tests
  std::vector<RedisConnChain> RedisConnectionList = cm.get_redisconnlist();
  int conn_list_size = RedisConnectionList.size();

  if (conn_list_size > 0) {
    RedisConnChain redis_chain = RedisConnectionList[0];
    assert( redis_chain.ip == "127.0.0.1" );
    assert( redis_chain.port == 6379 );
    assert( redis_chain.password == "test" );
    assert( redis_chain.pool_size == 2);
    assert( redis_chain.timeout == 5);
    assert( redis_chain.role == 0);
    logging->debug("Redis connection list 1 checked");
  }

  if (conn_list_size > 1) {
    RedisConnChain redis_chain2 = RedisConnectionList[1];
    assert( redis_chain2.ip == "127.0.0.1" );
    assert( redis_chain2.port == 6380 );
    assert( redis_chain2.password == "test2" );
    assert( redis_chain2.pool_size == 2);
    assert( redis_chain2.timeout == 5);
    assert( redis_chain2.role == 0);
    logging->debug("Redis connection list 2 checked");
  }

  shutdown_logging_submodules();

  delete cli;
  delete ua;
  delete logging;
  delete id_factory;
  delete cli_factory;
  delete logging_factory;
  return 0;
}
