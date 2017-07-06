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

#ifndef AOSSL_MONGO_INCLUDE_FACTORY_MONGO_H_
#define AOSSL_MONGO_INCLUDE_FACTORY_MONGO_H_

#include <string>
#include "mongo_interface.h"
#include "mongo_admin.h"

//! The Mongo Service Component Factory

//! The Service Component Factory tracks the Mongo
//! objects exposed by the framework and passes back
//! instances of interfaces.  This allows for the publicly exposed methods
//! to be independent of the implementations.
class MongoComponentFactory {
 public:
  //! Create a new Service Component Factory
  MongoComponentFactory() {}

  //! Delete a Service Component Factory
  ~MongoComponentFactory() {}

  //! Get a Mongo Interface instance
  inline MongoInterface* get_mongo_interface(const char * url, \
    const char * db, const char * collection_name) {
    return new MongoClient(url, db, collection_name);
  }

  //! Get a Mongo Interface instance
  inline MongoInterface* get_mongo_interface(std::string url, \
    std::string db, std::string collection_name) {
    return new MongoClient(url, db, collection_name);
  }

  //! Get a Mongo Interface instance
  inline MongoInterface* get_mongo_interface(const char * url, \
    const char * db) {
    return new MongoClient(url, db);
  }

  //! Get a Mongo Interface instance
  inline MongoInterface* get_mongo_interface(std::string url, std::string db) {
    return new MongoClient(url, db);
  }

  //! Get a Mongo Interface instance
  inline MongoInterface* get_mongo_interface(const char * url, \
    const char * db, const char * collection_name, int pool_size) {
    return new MongoClient(url, db, collection_name, pool_size);
  }

  //! Get a Mongo Interface instance
  inline MongoInterface* get_mongo_interface(std::string url, \
    std::string db, std::string collection_name, int pool_size) {
    return new MongoClient(url, db, collection_name, pool_size);
  }

  //! Get a Mongo Interface instance
  inline MongoInterface* get_mongo_interface(const char * url, \
    const char * db, int pool_size) {
    return new MongoClient(url, db, pool_size);
  }

  //! Get a Mongo Interface instance
  inline MongoInterface* get_mongo_interface(std::string url, \
    std::string db, int pool_size) {
    return new MongoClient(url, db, pool_size);
  }
};

#endif  // AOSSL_MONGO_INCLUDE_FACTORY_MONGO_H_
