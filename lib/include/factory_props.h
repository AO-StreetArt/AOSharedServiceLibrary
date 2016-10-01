#include <string.h>

#include "factory/properties_reader_interface.h"
#include "properties_reader.h"

#ifndef AOSSL_FACTORY_PROPFILE
#define AOSSL_FACTORY_PROPFILE

//! The Property File Reader Service Component Factory

//! The Service Component Factory tracks the Property File Reader
//! objects exposed by the framework and passes back
//! instances of interfaces.  This allows for the publicly exposed methods
//! to be independent of the implementations.
class PropertyReaderFactory
{
public:

  //! Create a new Service Component Factory
  PropertyReaderFactory() {}

  //! Delete a Service Component Factory
  ~PropertyReaderFactory() {}

  //! Get a Properties File Interface Instance
  PropertiesReaderInterface* get_properties_reader_interface(std::string filename) {return new PropertiesReader(filename);}
};

#endif
