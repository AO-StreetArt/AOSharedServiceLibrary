#include "include/zmqio.h"

//-------------------------Inbound ZMQ Admin----------------------------------//

//Constructor & Destructor
Zmqi::Zmqi(zmq::context_t &context)
{
  zmqi = new zmq::socket_t (context, ZMQ_REP);
}

Zmqi::~Zmqi()
{
  delete zmqi;
}

//Bind the inbound socket
void Zmqi::bind(std::string conn_str)
{
  zmqi->bind(conn_str);
}

//Recieve an inbound message
std::string Zmqi::recv()
{
  zmq::message_t request;

  //  Wait for next request from client
  zmqi->recv (&request);
  logging->info("ZMQ: Inbound Request Recieved");

  //Convert the OMQ message into a string to be passed
  std::string req_string;
  req_string = hexDump (request);
  logging->debug(req_string);
  return req_string;
}

//Send a response
void Zmqi::send(const char * msg, int msg_size)
{
  //  Send reply back to client
  zmq::message_t reply (msg_size);

  //Prepare return data
  memcpy (reply.data (), msg, msg_size);
  //Send the response
  zmqi->send (reply);
  logging->info("ZMQ: Inbound Response Sent");
  logging->debug(msg);
}

//Send a string response
void Zmqi::send_str(std::string msg)
{
  const char * msg_cstr = msg.c_str();
  send(msg_cstr, msg.size());
}

//-------------------------Outbound ZMQ Admin---------------------------------//

//Constructor & Destructor
Zmqo::Zmqo(zmq::context_t &context)
{
  zmqo = new zmq::socket_t (context, ZMQ_REQ);
}

Zmqo::~Zmqo()
{
  delete zmqo;
}

//Connect to the Socket
void Zmqo::connect(std::string conn_str)
{
  zmqo->connect(conn_str);
}

//Send a message
void Zmqo::send(const char * msg, int msg_size)
{
  //Set up the message to go out on 0MQ
  zmq::message_t req (msg_size);
  memcpy (req.data (), msg, msg_size);

  //Send the message
  zmqo->send (req);

  logging->info("ZMQ: Outbound Message Sent");
  logging->debug(msg);
}

//Send a string message
void Zmqo::send_str(std::string msg) {
  const char * msg_cstr = msg.c_str();
  send(msg_cstr, msg.size());
}

//Recieve a Response
std::string Zmqo::recv()
{
  //  Get the reply.
  zmq::message_t rep;
  zmqo->recv (&rep);

  logging->info("ZMQ: Outbound Response Recieved");

  //Process the reply
  std::string r_str = hexDump(rep);
  logging->debug(r_str);
  return r_str;
}
