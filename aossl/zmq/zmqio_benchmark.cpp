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

#include <hayai/hayai.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <string>

#include "include/zmq_interface.h"
#include "include/factory_zmq.h"

Zmqio *zmqo;
Zmqio *zmqi;

// Benchmarks

BENCHMARK(ZMQ, SendRecieve, 10, 100) {
  // Send a Message
  std::string msg = "Test";
  zmqo->send(msg);
  bool keep_going = true;

  while (keep_going) {
    // Convert the OMQ message into a string to be passed on the event
    std::string req_string = zmqi->recv();

    std::string resp = "success";
    std::cout << "Object Update Event Emitted, response:" << resp << std::endl;

    // Send reply back to client
    zmqi->send(resp);
    std::cout << "Response Sent" << std::endl;

    keep_going = false;
  }

  std::string response = zmqo->recv();
  std::cout << "Response Recieved" << response << std::endl;
}

int main() {
  ZmqComponentFactory zmq_factory;

  // Set up UUID Generator
  zmqo = \
    zmq_factory.get_zmq_outbound_interface("tcp://localhost:5555", REQ_RESP);
  zmqi = zmq_factory.get_zmq_inbound_interface("tcp://*:5555", REQ_RESP);

  // Run Tests

  hayai::ConsoleOutputter consoleOutputter;

  hayai::Benchmarker::AddOutputter(consoleOutputter);
  hayai::Benchmarker::RunAllTests();

  // Cleanup

  delete zmqo;
  delete zmqi;

  return 0;
}
