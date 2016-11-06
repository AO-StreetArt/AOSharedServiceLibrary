#include "factory/mongo_interface.h"
#include "mongo_admin.h"

#ifndef AOSSL_FACTORY_MONGO
#define AOSSL_FACTORY_MONGO

//! The Mongo Service Component Factory

//! The Service Component Factory tracks the Mongo
//! objects exposed by the framework and passes back
//! instances of interfaces.  This allows for the publicly exposed methods
//! to be independent of the implementations.
class MongoComponentFactory
{
public:

  //! Create a new Service Component Factory
  MongoComponentFactory() {}

  //! Delete a Service Component Factory
  ~MongoComponentFactory() {}

  //! Get a Couchbase Interface instance
  MongoInterface* get_mongo_interface(const char * url, const char * db, const char * collection_name) {return new MongoClient(url, db, collection_name);}
  MongoInterface* get_mongo_interface(std::string url, std::string db, std::string collection_name) {return new MongoClient(url, db, collection_name);}
  MongoInterface* get_mongo_interface(const char * url, const char * db) {return new MongoClient(url, db);}
  MongoInterface* get_mongo_interface(std::string url, std::string db) {return new MongoClient(url, db);}

};

#endif
