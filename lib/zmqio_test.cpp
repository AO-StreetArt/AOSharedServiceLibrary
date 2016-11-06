//Tests for the Zmqio Module

#include "include/factory_zmq.h"
#include "include/factory/zmq_interface.h"

#include <iostream>

int main()
{

  //Set up the underlying variables
  Zmqio *zmqo;
  Zmqio *zmqi;
  ZmqComponentFactory zmq_factory;

  //Set up the ZMQ Clients
  zmqo = zmq_factory.get_zmq_outbound_interface("tcp://localhost:5555", REQ_RESP);
  zmqi = zmq_factory.get_zmq_inbound_interface("tcp://*:5555", REQ_RESP);

  //Send a Message
  std::string msg = "Test";
  zmqo->send(msg);
  bool keep_going = true;

  while (keep_going) {

    //Convert the OMQ message into a string to be passed on the event
    std::string req_string = zmqi->recv();
    std::cout << req_string << std::endl;
    assert ( req_string == "Test" );
    std::string resp = "success";
    //  Send reply back to client
    zmqi->send(resp);

    keep_going = false;

  }

  std::string response = zmqo->recv();
  std::cout << response << std::endl;
  assert ( response == "success" );

  //Cleanup
  delete zmqi;
  delete zmqo;

  return 0;

}
