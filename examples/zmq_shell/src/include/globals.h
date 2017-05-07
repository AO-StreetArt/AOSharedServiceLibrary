//This defines some of the global variables that need to be accessed
//in a globally defined callback that takes no input parameters.
//This callback is defined in the main file and, when triggered by a signal,
//ensures that all of the resources allocated from AOSSL are deleted correctly.

#include "aossl/commandline/include/commandline_interface.h"
#include "aossl/commandline/include/factory_cli.h"

#include "aossl/mongo/include/mongo_interface.h"
#include "aossl/mongo/include/factory_mongo.h"

#include "aossl/logging/include/logging_interface.h"
#include "aossl/logging/include/factory_logging.h"

#include "aossl/redis/include/redis_interface.h"
#include "aossl/redis/include/factory_redis.h"

#include "aossl/uuid/include/uuid_interface.h"
#include "aossl/uuid/include/factory_uuid.h"

#include "aossl/zmq/include/zmq_interface.h"
#include "aossl/zmq/include/factory_zmq.h"

#include "configuration_manager.h"
#include "app_log.h"

#ifndef GLOBALS
#define GLOBALS

//Globals defined within this service
extern ConfigurationManager *config;

//Globals from the AO Shared Service Library
extern MongoInterface *mongo;
extern RedisInterface *red;
extern uuidInterface *uid;
extern Zmqio *zmqi;
extern CommandLineInterface *cli;

//Global Factory Objects
extern CommandLineInterpreterFactory *cli_factory;
extern MongoComponentFactory *mongo_factory;
extern RedisComponentFactory *redis_factory;
extern uuidComponentFactory *uuid_factory;
extern ZmqComponentFactory *zmq_factory;
extern LoggingComponentFactory *logging_factory;

//Shutdown the application
inline void shutdown()
{
  //Delete core objects
  if (mongo) delete mongo;
  if (red) delete red;
  if (zmqi) delete zmqi;
  if (config) delete config;
  if (uid) delete uid;
  if (cli) delete cli;

  //Shutdown logging
  shutdown_logging_submodules();
  if (logging) delete logging;

  //Shut down protocol buffer library

  //Uncomment this line if you are using protocol buffers
  //google::protobuf::ShutdownProtobufLibrary();

  //Delete factories
  if (cli_factory) delete cli_factory;
  if (mongo_factory) delete mongo_factory;
  if (redis_factory) delete redis_factory;
  if (uuid_factory) delete uuid_factory;
  if (zmq_factory) delete zmq_factory;
  if (logging_factory) delete logging_factory;
}

#endif
