#include "factory/couchbase_interface.h"
#include "couchbase_admin.h"

#ifndef AOSSL_FACTORY_COUCHBASE
#define AOSSL_FACTORY_COUCHBASE

//! The Couchbase Service Component Factory

//! The Service Component Factory tracks the Couchbase
//! objects exposed by the framework and passes back
//! instances of interfaces.  This allows for the publicly exposed methods
//! to be independent of the implementations.
class CouchbaseComponentFactory
{
public:

  //! Create a new Service Component Factory
  CouchbaseComponentFactory() {}

  //! Delete a Service Component Factory
  ~CouchbaseComponentFactory() {}

  //! Get a Couchbase Interface instance
  CouchbaseInterface* get_couchbase_interface( const char * conn ) {return new CouchbaseAdmin( conn );}

  //! Get a Couchbase Interface instance for a password protected DB
  CouchbaseInterface* get_couchbase_interface( const char * conn, const char * pswd ) {return new CouchbaseAdmin( conn, pswd );}

};

#endif
