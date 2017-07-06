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

#ifndef AOSSL_ZMQ_INCLUDE_ZMQIO_H_
#define AOSSL_ZMQ_INCLUDE_ZMQIO_H_

#include <zmq.hpp>
#include <stdlib.h>
#include <string>
#include <mutex>
#include "zmq_interface.h"

// An Outbound ZMQ Manager

// Acts as the Requestor (Client) in the ZMQ Sockets
// Send, then Recieve
class Zmqo: public ZmqOut {
  int conn_type;
  zmq::socket_t *zmqo;
  zmq::message_t request;
  std::mutex send_mutex;
  std::string r_str;
  const char * msg_cstr;
  char * rcv_cstr = NULL;
  bool started = false;

 public:
  // Build a new Outbound ZMQ Manager
  Zmqo(zmq::context_t &context, int connection_type);

  // Destroy the ZMQO Manager
  ~Zmqo();

  // Connect to the given conn_str
  void connect(std::string conn_str);

  // Send a message on the port
  void send(const char * msg, int msg_size);

  // Send a string on the port
  void send(std::string msg);

  // Recieve a message on the port
  std::string recv();

  // Recieve a message on the port
  char * crecv();

  void subscribe(std::string filter) {}
};

// An Inbound ZMQ Manager

// Acts as the Responder (Server) in the ZMQ Sockets
// Recieve, then Send
class Zmqi: public ZmqIn {
  int conn_type;
  zmq::socket_t *zmqi;
  // Perhaps we need to store this as a pointer?
  zmq::message_t *request = NULL;
  std::string req_string;
  const char * msg_cstr;
  char * rcv_cstr = NULL;
  bool started = false;

 public:
  // Build a new Inbound ZMQ Manager
  Zmqi(zmq::context_t &context, int connection_type);

  // Destroy the ZMQI Manager
  ~Zmqi();

  // Bind on the given conn_str
  void bind(std::string conn_str);

  // Recieve a message on the port
  std::string recv();

  // Recieve a message on the port
  char * crecv();

  // Send a message on the port
  void send(const char * msg, int msg_size);

  // Send a string on the port
  void send(std::string msg);

  // Subscribe on a filter
  void subscribe(std::string filter);
};
#endif  // AOSSL_ZMQ_INCLUDE_ZMQIO_H_
