#include <zmq.hpp>
#include "zmqio.h"
#include "factory/zmq_interface.h"

#ifndef AOSSL_FACTORY_ZMQ
#define AOSSL_FACTORY_ZMQ

//! The ZMQ Service Component Factory

//! The Service Component Factory tracks the ZMQ
//! objects exposed by the framework and passes back
//! instances of interfaces.  This allows for the publicly exposed methods
//! to be independent of the implementations.
class ZmqComponentFactory
{
zmq::context_t *context;
public:

  //! Create a new Service Component Factory
  ZmqComponentFactory() {context = new zmq::context_t(1, 2);}
  ZmqComponentFactory(int num_sockets) {context = new zmq::context_t(1, num_sockets);}

  //! Delete a Service Component Factory
  ~ZmqComponentFactory() {delete context;}

  //! Get a ZMQ Outbound Interface instance
  inline Zmqio* get_zmq_outbound_interface( std::string conn_str, int connection_type )
  {
    ZmqOut *zmqo =  new Zmqo( *context, connection_type );
    zmqo->connect( conn_str );
    return zmqo;
  }

  //! Get a ZMQ Inbound Interface instance
  inline Zmqio* get_zmq_inbound_interface( std::string conn_str, int connection_type )
  {
    ZmqIn *zmqi = new Zmqi( *context, connection_type );
    zmqi->bind( conn_str );
    return zmqi;
  }

};

#endif
