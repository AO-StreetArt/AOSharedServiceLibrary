//Tests for the Zmqio Module

#include "zmqio.h"

int main()
{
  //Set up logging
  std::string initFileName = "test/log4cpp_test.properties";
  try {
  	log4cpp::PropertyConfigurator::configure(initFileName);
  }
  catch ( log4cpp::ConfigureFailure &e ) {
  	printf("[log4cpp::ConfigureFailure] caught while reading Logging Configuration File");
  	printf(e.what());
  	exit(1);
  }

  log4cpp::Category& root = log4cpp::Category::getRoot();

  log4cpp::Category& sub1 = log4cpp::Category::getInstance(std::string("sub1"));

  log4cpp::Category& log = log4cpp::Category::getInstance(std::string("sub1.log"));

  logging = &log;

  //Set up the outbound ZMQ Client
  zmqo = new Zmqo (context);
  logging->info("0MQ Constructor Called");
  zmqo->connect("tcp://localhost:5555");
  logging->info("Connected to Outbound OMQ Socket");

  //Connect to the inbound ZMQ Socket
  zmqi = new Zmqi (context);
  logging->info("0MQ Constructor Called");
  zmqi->bind("tcp://*:5555");
  logging->info("ZMQ Socket Open, opening request loop");

  //Send a Message
  zmqo->send_str("Test");
  bool keep_going = true;

  while (keep_going) {

    //Convert the OMQ message into a string to be passed on the event
    std::string req_string = zmqi->recv();

    resp = "success";
    logging->debug("Object Update Event Emitted, response:");
    logging->debug(resp);

    //  Send reply back to client
    zmqi->send_str(resp);
    logging->debug("Response Sent");

    keep_going = false;

  std::string response = zmqo->recv();
  logging->debug("Response Recieved");
  logging->debug(response);

  //Cleanup
  end_log();
  delete zmqi;
  delete zmqo;

  return 0;

}
