//This sets up all of the components necessary for the service and runs the main
//loop for the application.

#include <sstream>
#include <string>
#include <string.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <unistd.h>
#include <exception>
#include <signal.h>

#include "src/include/app_log.h"
#include "src/include/app_utils.h"
#include "src/include/configuration_manager.h"
#include "src/include/globals.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"

#include "aossl/commandline/include/commandline_interface.h"
#include "aossl/commandline/include/factory_cli.h"

#include "aossl/logging/include/logging_interface.h"
#include "aossl/logging/include/factory_logging.h"

#include "aossl/redis/include/redis_interface.h"
#include "aossl/redis/include/factory_redis.h"

#include "aossl/uuid/include/uuid_interface.h"
#include "aossl/uuid/include/factory_uuid.h"

#include "aossl/zmq/include/zmq_interface.h"
#include "aossl/zmq/include/factory_zmq.h"

//Catch a Signal (for example, keyboard interrupt)
void my_signal_handler(int s){
   main_logging->error("Caught signal");
   std::string signal_type = std::to_string(s);
   main_logging->error(signal_type);
   shutdown();
   exit(1);
}

    //-----------------------
    //------Main Method------
    //-----------------------

    int main( int argc, char** argv )
    {

      //Set up a handler for any signal events so that we always shutdown gracefully
      struct sigaction sigIntHandler;
      sigIntHandler.sa_handler = my_signal_handler;
      sigemptyset(&sigIntHandler.sa_mask);
      sigIntHandler.sa_flags = 0;

      sigaction(SIGINT, &sigIntHandler, NULL);

      cli_factory = new CommandLineInterpreterFactory;
      redis_factory = new RedisComponentFactory;
      uuid_factory = new uuidComponentFactory;
      zmq_factory = new ZmqComponentFactory;
      logging_factory = new LoggingComponentFactory;
      mongo_factory = new MongoComponentFactory;

      //Set up our command line interpreter
      cli = cli_factory->get_command_line_interface( argc, argv );

      //Set up logging
	    std::string initFileName;

      //See if we have a command line setting for the log file
      const char * env_logging_file = std::getenv("CRAZYIVAN_LOGGING_CONF");
      if ( env_logging_file ) {
        std::string tempFileName (env_logging_file);
        initFileName = tempFileName;
      }
      else if ( cli->opt_exist("-log-conf") ) {
        initFileName = cli->get_opt("-log-conf");
      }
      else
      {
        initFileName = "log4cpp.properties";
      }

      //This reads the logging configuration file
      logging = logging_factory->get_logging_interface(initFileName);

      //Set up the logging submodules for each category
      start_logging_submodules();

      //Set up the UUID Generator
      uid = uuid_factory->get_uuid_interface();

      std::string service_instance_id = "Ivan-";
      UuidContainer sid_container;
      try {
        sid_container = uid->generate();
        if (!sid_container.err.empty()) {
          main_logging->error(sid_container.err);
        }
        service_instance_id = service_instance_id + sid_container.id;
      }
      catch (std::exception& e) {
        main_logging->error("Exception encountered during Service Instance ID Generation");
        shutdown();
        exit(1);
      }

      //Set up our configuration manager with the CLI
      config = new ConfigurationManager(cli, service_instance_id);

      //The configuration manager will  look at any command line arguments,
      //configuration files, and Consul connections to try and determine the correct
      //configuration for the service

      bool config_success = false;
      try {
        config_success = config->configure();
      }
      catch (std::exception& e) {
        main_logging->error("Exception encountered during Configuration");
        shutdown();
        exit(1);
      }
      if (!config_success)
      {
        main_logging->error("Configuration Failed, defaults kept");
      }

      //Set up our Redis Connection List, which is passed to the Redis Admin to connect
      std::vector<RedisConnChain> RedisConnectionList = config->get_redisconnlist();
      //Set up Redis Connection
      if (RedisConnectionList.size() > 0) {
        try {
          //Currently only support for single Redis instance
          red = redis_factory->get_redis_interface(RedisConnectionList[0].ip, RedisConnectionList[0].port);
        }
        catch (std::exception& e) {
          main_logging->error("Exception encountered during Redis Initialization");
          main_logging->error(e.what());
          shutdown();
          exit(1);
        }
        main_logging->info("Connected to Redis");
      }
      else {
        main_logging->error("No Redis Connections found in configuration");
      }

      //Set up the Mongo Connection
      std::string DBConnStr = config->get_mongoconnstr();
      std::string DBName = config->get_dbname();
      std::string DBHeaderCollection = config->get_dbheadercollection();
      if ( !(DBConnStr.empty() || DBName.empty() || DBHeaderCollection.empty()) ) {
        try {
          mongo = mongo_factory->get_mongo_interface( DBConnStr, DBName, DBHeaderCollection );
          main_logging->debug("Connected to Mongo");
        }
        catch (std::exception& e) {
          main_logging->error("Exception encountered during Mongo Initialization");
          main_logging->error(e.what());
          shutdown();
          exit(1);
        }
      }
      else {
        main_logging->error("Insufficient Mongo Connection Information Supplied");
        shutdown();
        exit(1);
      }

      //Connect to the inbound ZMQ Admin
      std::string ib_zmq_connstr = config->get_ibconnstr();
      if ( !(ib_zmq_connstr.empty()) ) {
        zmqi = zmq_factory->get_zmq_inbound_interface(ib_zmq_connstr, REQ_RESP);
        main_logging->info("ZMQ Socket Open, opening request loop");
      }
      else {
        main_logging->error("No IB ZMQ Connection String Supplied");
        shutdown();
        exit(1);
      }

      //Main Request Loop

      while (true) {

        std::string resp_str = "";
        rapidjson::Document d;

        //Convert the OMQ message into a string to be passed on the event
        char * req_ptr = zmqi->crecv();
        main_logging->debug("Conversion to C String performed with result: ");
        main_logging->debug(req_ptr);

        //Trim the string recieved
        std::string recvd_msg (req_ptr);
        std::string clean_string = trim(recvd_msg);

        main_logging->debug("Input String Cleaned");
        main_logging->debug(clean_string);

        if (config->get_formattype() == JSON_FORMAT) {

          clean_string = recvd_msg.substr(0, recvd_msg.find_last_of("}")+1);

          try {
            d.Parse<rapidjson::kParseStopWhenDoneFlag>(clean_string.c_str());
            if (d.HasParseError()) {
              main_logging->error("Parsing Error: ");
              main_logging->error(GetParseError_En(d.GetParseError()));
            }
          }
          //Catch a possible error and write to logs
          catch (std::exception& e) {
            main_logging->error("Exception occurred while parsing inbound document:");
            main_logging->error(e.what());
          }

        }

          //Determine the Transaction ID
          UuidContainer id_container;
          id_container.id = "";
          if ( config->get_transactionidsactive() ) {
            //Get an existing transaction ID, currently empty as we don't assume format
            std::string existing_trans_id = "";
            //If no transaction ID is sent in, generate a new one
            if ( existing_trans_id.empty() ) {
              try {
                id_container = uid->generate();
                if (!id_container.err.empty()) {
                  main_logging->error(id_container.err);
                }
                main_logging->debug("Generated Transaction ID: " + id_container.id);

              }
              catch (std::exception& e) {
                main_logging->error("Exception encountered during UUID Generation");
                shutdown();
                exit(1);
              }
            }
            //Otherwise, use the existing transaction ID
            else {
              id_container.id = existing_trans_id;
            }
          }
          main_logging->debug("Transaction ID: ");
          main_logging->debug(id_container.id);

          //Core application logic

          //Here we have the logic to build a response.
          //For the purposes of this shell, we will simply assign
          //the transaction ID to the response and send it back
          std::string application_response = id_container.id;

          main_logging->info("Sending Response");
          main_logging->info( application_response );
          zmqi->send( application_response );

      }
      return 0;
    }
