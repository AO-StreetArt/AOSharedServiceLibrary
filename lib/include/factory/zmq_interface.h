#include <string>

#ifndef ZMQ_INTERFACE
#define ZMQ_INTERFACE

//! An Interface for ZMQIO

//! Defines the methods that the Req/Resp ZMQ Managers must implement
//! send & recv
class Zmqio
{
public:
  virtual ~Zmqio() = 0;
  //! Recieve a message on the port
  virtual std::string recv() = 0;

  //! Send a message on the port
  virtual void send(const char * msg, int msg_size) = 0;

  //! Send a string on the port
  virtual void send(std::string msg) = 0;
};

//! An Outbound ZMQ Manager

//! Acts as the Requestor (Client) in the ZMQ Sockets
//! Send, then Recieve
class ZmqOut: public Zmqio
{
public:

  virtual ~ZmqOut() = 0;

  //! Connect to the given conn_str
  virtual void connect(std::string conn_str) = 0;

  //! Send a message on the port
  virtual void send(const char * msg, int msg_size) = 0;

  //! Send a string on the port
  virtual void send(std::string msg) = 0;

  //! Recieve a message on the port
  virtual std::string recv() = 0;
};

//! An Inbound ZMQ Manager

//! Acts as the Responder (Server) in the ZMQ Sockets
//! Recieve, then Send
class ZmqIn: public Zmqio
{
public:

  virtual ~ZmqIn() = 0;

  //! Bind on the given conn_str
  virtual void bind(std::string conn_str) = 0;

  //! Recieve a message on the port
  virtual std::string recv() = 0;

  //! Send a message on the port
  virtual void send(const char * msg, int msg_size) = 0;

  //! Send a string on the port
  virtual void send(std::string msg) = 0;
};
#endif
