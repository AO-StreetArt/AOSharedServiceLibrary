#include "zmqo.h"

zmq::socket_t *zmqo;

void send_zmqo_message(const char * msg, int msg_size)
{

  //Set up the message to go out on 0MQ
  zmq::message_t req (msg_size);
  memcpy (req.data (), msg, msg_size);

  //Send the message
  zmqo->send (req);

  //  Get the reply.
  zmq::message_t rep;
  zmq::message_t *rep_ptr;
  //rep_ptr = &reply;
  zmqo->recv (&rep);

  //Process the reply
  std::string r_str = hexDump(rep);

  logging->info("ZMQ:Message Sent:");
  logging->info(msg);
  logging->info("ZMQ:Response Recieved:");
  logging->info(r_str);
}

void send_zmqo_str_message(std::string &msg) {
  const char * msg_cstr = msg.c_str();
  send_zmqo_message(msg_cstr, msg.size());
}
