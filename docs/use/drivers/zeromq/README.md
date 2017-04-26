# ZeroMQ Sockets

ZeroMQ is a permanent-connection alternative that provides high-speed, distributed messaging.

This provides Request/Reply Managers for both Inbound and Outbound sockets.

The Zmqio object exposes these methods:

* crecv() - a blocking call to wait for a message, and return a C-style string
* recv() - a blocking call to wait for a message, and return a std::string
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

# Thread Safety

The outbound and inbound ZeroMQ Managers both have a mutex around the send() functions.

When using the recv() function with multiple threads, keep in mind that the resources allocated in the message MAY NOT BE PRESENT once the next message has been received.  You should ensure that you take a deep copy of the message prior to passing off to another thread.
