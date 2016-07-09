#include <string>
#include <stdlib.h>
#include <zmq.hpp>
#include "lyman_utils.h"
#include "logging.h"

#ifndef ZMQO
#define ZMQO

//Global Outbound ZMQ Dispatcher
extern zmq::socket_t *zmqo;

//Send Outbound Messages
void send_zmqo_message(const char * msg, int msg_size);

void send_zmqo_str_message(std::string &msg);

#endif
