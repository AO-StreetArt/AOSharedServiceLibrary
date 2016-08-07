//Tests for the Zmqio Module

#include "include/zmqio.h"

int main()
{
  //Set up logging
  std::string initFileName = "test/log4cpp_test.properties";
  logging = new Logger(initFileName);

  //Set up the underlying variables
  Zmqo *zmqo;
  Zmqi *zmqi;

  //We maintain the ZMQ Context and pass it to the ZMQ objects coming from aossl
  zmq::context_t context(1, 2);

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
  std::string msg = "Test";
  zmqo->send_str(msg);
  bool keep_going = true;

  while (keep_going) {

    //Convert the OMQ message into a string to be passed on the event
    std::string req_string = zmqi->recv();

    std::string resp = "success";
    logging->debug("Object Update Event Emitted, response:");
    logging->debug(resp);

    //  Send reply back to client
    zmqi->send_str(resp);
    logging->debug("Response Sent");

    keep_going = false;

  }

  std::string response = zmqo->recv();
  logging->debug("Response Recieved");
  logging->debug(response);

  //Cleanup
  end_log();
  delete zmqi;
  delete zmqo;

  return 0;

}
