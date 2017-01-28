# ZeroMQ Sockets

ZeroMQ is a permanent-connection alternative that provides high-speed, distributed messaging.

This provides Request/Reply Managers for both Inbound and Outbound sockets.

The Zmqio object exposes these methods:

* recv() - a blocking call to wait for a message
* send(const char * msg, int msg_size)
* send(std::string msg)

In order to connect to a socket, we ask the factory to create one:

    #include "aossl/zmq/zmq_interface.h"
    #include "aossl/zmq/factory_zmq.h"

    ZmqComponentFactory zmq_factory;

    //Set up the outbound ZMQ Client
    Zmqio *zmqo = zmq_factory.get_zmq_outbound_interface("tcp://localhost:5555");

    //Set up the inbound ZMQ Client
    Zmqio *zmqi = zmq_factory.get_zmq_inbound_interface("tcp://*:5555");
