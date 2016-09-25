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
  ConsulComponentFactory();

  //! Delete a Service Component Factory
  ~ConsulComponentFactory();

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
