/*
MIT License Block

Copyright (c) 2016 Alex Barry

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// Tests for the Zmqio Module

#include <iostream>
#include <string>

#include "include/factory_zmq.h"
#include "include/zmq_interface.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {return -1;}

  std::string con_port(argv[1]);
  std::string con_str = "tcp://*:" + con_port;

  // Set up the ZMQ Connection
  Zmqio *zmqi;
  ZmqComponentFactory zmq_factory;

  // Set up the ZMQ Clients
  zmqi = zmq_factory.get_zmq_inbound_interface(con_str, REQ_RESP);


  while (true) {
    // Convert the OMQ message into a string to be passed on the event
    char * req_cstr = zmqi->crecv();
    std::cout << req_cstr << std::endl;
    std::string resp = "success";
    //   Send reply back to client
    zmqi->send(resp);
  }

  // Cleanup
  delete zmqi;

  return 0;
}
