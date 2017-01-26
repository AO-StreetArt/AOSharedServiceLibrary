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

#include <string.h>
#include "factory/consul_interface.h"
#include "service.h"
#include "consul_admin.h"

#ifndef AOSSL_FACTORY_CONSUL
#define AOSSL_FACTORY_CONSUL

//! The Consul Service Component Factory

//! The Service Component Factory tracks the Consul
//! objects exposed by the framework and passes back
//! instances of interfaces.  This allows for the publicly exposed methods
//! to be independent of the implementations.
class ConsulComponentFactory
{
public:

  //! Create a new Service Component Factory
  ConsulComponentFactory() {}

  //! Delete a Service Component Factory
  ~ConsulComponentFactory() {}

  //! Get a Consul Interface instance
  ConsulInterface* get_consul_interface( std::string caddr ) {return new ConsulAdmin( caddr );}

  ServiceInterface* get_service_interface() {return new Service;}

  //! Get a Service Interface instance
  ServiceInterface* get_service_interface(std::string new_id, std::string new_name) {return new Service( new_id, new_name );}

  //! Get a Service Interface instance
  ServiceInterface* get_service_interface(std::string new_id, std::string new_name, std::string new_address, std::string new_port) {return new Service( new_id, new_name, new_address, new_port );}

  //! Get a Service Interface instance
  ServiceInterface* get_service_interface(std::string new_id, std::string new_name, std::string new_address, std::string new_port, std::vector<std::string> new_tags) {return new Service( new_id, new_name, new_address, new_port, new_tags );}

};

#endif
