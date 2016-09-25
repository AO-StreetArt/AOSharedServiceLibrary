#include <hayai/hayai.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdlib>

#include "include/factory/logging_interface.h"
#include "include/factory/zmq_interface.h"
#include "include/factory_logging.h"
#include "include/factory_zmq.h"

Zmqio *zmqo;
Zmqio *zmqi;

//----------------------------------------------------------------------------//
//------------------------------Benchmarks------------------------------------//
//----------------------------------------------------------------------------//

BENCHMARK(ZMQ, SendRecieve, 10, 100)
{

  //Send a Message
  std::string msg = "Test";
  zmqo->send(msg);
  bool keep_going = true;

  while (keep_going) {

    //Convert the OMQ message into a string to be passed on the event
    std::string req_string = zmqi->recv();

    std::string resp = "success";
    logging->debug("Object Update Event Emitted, response:");
    logging->debug(resp);

    //  Send reply back to client
    zmqi->send(resp);
    logging->debug("Response Sent");

    keep_going = false;

  }

  std::string response = zmqo->recv();
  logging->debug("Response Recieved");
  logging->debug(response);

}

//----------------------------------------------------------------------------//
//------------------------------Main Method-----------------------------------//
//----------------------------------------------------------------------------//

int main()
{

  ZmqComponentFactory zmq_factory;
  LoggingComponentFactory logging_factory;

//Read the Logging Configuration File
std::string initFileName = "test/log4cpp_test.properties";
logging = logging_factory.get_logging_interface( initFileName );
//logging = new Logger(initFileName);

//Set up internal variables
logging->info("Internal Logging Intialized");

//Set up UUID Generator
zmqo = zmq_factory.get_zmq_outbound_interface( "tcp://localhost:5555" );
zmqi = zmq_factory.get_zmq_inbound_interface( "tcp://*:5555" );

//------------------------------Run Tests-------------------------------------//
//----------------------------------------------------------------------------//

hayai::ConsoleOutputter consoleOutputter;

hayai::Benchmarker::AddOutputter(consoleOutputter);
hayai::Benchmarker::RunAllTests();

//-------------------------Post-Test Teardown---------------------------------//
//----------------------------------------------------------------------------//

delete zmqo;
delete zmqi;
delete zmq_logging;
delete logging;

return 0;

}
