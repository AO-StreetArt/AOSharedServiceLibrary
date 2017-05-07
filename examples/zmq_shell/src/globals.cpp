#include "include/globals.h"

//Core objects
uuidInterface *uid = NULL;
ConfigurationManager *config = NULL;
MongoInterface *mongo = NULL;
RedisInterface *red = NULL;
CommandLineInterface *cli = NULL;
Zmqio *zmqi = NULL;

//Factories
CommandLineInterpreterFactory *cli_factory = NULL;
MongoComponentFactory *mongo_factory = NULL;
RedisComponentFactory *redis_factory = NULL;
uuidComponentFactory *uuid_factory = NULL;
ZmqComponentFactory *zmq_factory = NULL;
LoggingComponentFactory *logging_factory = NULL;
