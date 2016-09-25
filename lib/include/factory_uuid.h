#include "factory/uuid_interface.h"
#include "uuid_admin.h"

#ifndef AOSSL_FACTORY_UUID
#define AOSSL_FACTORY_UUID

//! The UUID Service Component Factory

//! The Service Component Factory tracks the UUID
//! objects exposed by the framework and passes back
//! instances of interfaces.  This allows for the publicly exposed methods
//! to be independent of the implementations.
class uuidComponentFactory
{
public:

  //! Create a new Service Component Factory
  uuidComponentFactory() {}

  //! Delete a Service Component Factory
  ~uuidComponentFactory() {}

  //! Get the UUID Interface instance
  uuidInterface* get_uuid_interface() {return new uuidAdmin;}

};

#endif
