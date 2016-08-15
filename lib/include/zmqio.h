#include <string>
#include <stdlib.h>
#include <zmq.hpp>

#include "factory/logging_interface.h"
#include "factory/zmq_interface.h"

#ifndef ZMQIO
#define ZMQIO

//! Convert a ZMQ Message to a std::string
inline std::string hexDump ( zmq::message_t &aMessage ) {
return std::string(static_cast<char*>(aMessage.data()), aMessage.size());
}

//! An Outbound ZMQ Manager

//! Acts as the Requestor (Client) in the ZMQ Sockets
//! Send, then Recieve
class Zmqo: public ZmqOut
{
zmq::socket_t *zmqo;
public:
  //! Build a new Outbound ZMQ Manager
  Zmqo(zmq::context_t &context);

  //! Destroy the ZMQO Manager
  ~Zmqo();

  //! Connect to the given conn_str
  void connect(std::string conn_str);

  //! Send a message on the port
  void send(const char * msg, int msg_size);

  //! Send a string on the port
  void send(std::string msg);

  //! Recieve a message on the port
  std::string recv();
};

//! An Inbound ZMQ Manager

//! Acts as the Responder (Server) in the ZMQ Sockets
//! Recieve, then Send
class Zmqi: public ZmqIn
{
zmq::socket_t *zmqi;
public:
  //! Build a new Inbound ZMQ Manager
  Zmqi(zmq::context_t &context);

  //! Destroy the ZMQI Manager
  ~Zmqi();

  //! Bind on the given conn_str
  void bind(std::string conn_str);

  //! Recieve a message on the port
  std::string recv();

  //! Send a message on the port
  void send(const char * msg, int msg_size);

  //! Send a string on the port
  void send(std::string msg);
};
#endif
