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
