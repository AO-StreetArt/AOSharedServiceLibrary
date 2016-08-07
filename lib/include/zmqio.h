#include <string>
#include <stdlib.h>
#include <zmq.hpp>
#include "logging.h"

#ifndef ZMQIO
#define ZMQIO

//Convert a ZMQ Message to a std::string
inline std::string hexDump ( zmq::message_t &aMessage ) {
return std::string(static_cast<char*>(aMessage.data()), aMessage.size());
}

class Zmqio
{
public:
  virtual std::string recv() = 0;
  virtual void send(const char * msg, int msg_size) = 0;
  virtual void send(std::string msg) = 0;
};

class Zmqo: public Zmqio
{
zmq::socket_t *zmqo;
public:
  Zmqo(zmq::context_t &context);
  ~Zmqo();
  void connect(std::string conn_str);
  void send(const char * msg, int msg_size);
  void send(std::string msg);
  std::string recv();
};

class Zmqi: public Zmqio
{
zmq::socket_t *zmqi;
public:
  Zmqi(zmq::context_t &context);
  ~Zmqi();
  void bind(std::string conn_str);
  std::string recv();
  void send(const char * msg, int msg_size);
  void send(std::string msg);
};
#endif
