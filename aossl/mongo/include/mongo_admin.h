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

// Stable, C++11 Mongo Client

#ifndef AOSSL_MONGO_INCLUDE_MONGO_ADMIN_H_
#define AOSSL_MONGO_INCLUDE_MONGO_ADMIN_H_

#include <bson.h>
#include <mongoc.h>
#include <string>
#include <exception>
#include <vector>
#include <mutex>

#include "mongo_interface.h"

const int MONGO_RESPONSE_CRT = 0;
const int MONGO_RESPONSE_OTHER = 1;

// Connection Pooling

// A struct containing the objects needed to run a query
struct MongoSession {
  mongoc_client_t *connection = NULL;
  mongoc_collection_t *collection = NULL;
  int index = -1;
};

// A Connection pool to ensure thread safety
class MongoConnectionPool {
  // A pool of neo4j connections
  std::vector<MongoSession> connections;
  // Array of ints (0/1) which determine which connections are open vs closed
  int *slots;
  // Internal integers
  int connection_limit = 1;
  int start_connections = 1;
  int current_connection = -1;
  int current_max_connection = 1;
  int connection_creation_batch = 1;
  std::string connection_string;
  std::string db_name_string;
  std::string db_coll_string;
  std::mutex get_conn_mutex;
  void init_slots();
  void init_connections(std::string conn_str, std::string db, std::string coll);

 public:
  // Constructors
  inline MongoConnectionPool(int size, std::string conn_str, std::string db, \
    std::string coll) {
    connection_limit = size;
    init_slots();
    init_connections(conn_str, db, coll);
  }

  inline MongoConnectionPool(int size, std::string conn_str, std::string db, \
    std::string coll, int start_conns) {
    init_slots();
    start_connections = start_conns;
    connection_limit = size;
    init_connections(conn_str, db, coll);
  }

  inline MongoConnectionPool(int size, std::string conn_str, std::string db, \
    std::string coll, int start_conns, int batch_size) {
    init_slots();
    start_connections = start_conns;
    connection_limit = size;
    connection_creation_batch = batch_size;
    init_connections(conn_str, db, coll);
  }

  ~MongoConnectionPool();
  MongoSession* get_connection();
  void release_connection(MongoSession *conn);
  inline void switch_collection(std::string new_col) {db_coll_string = new_col;}
  inline void switch_database(std::string new_db) {db_name_string = new_db;}
  inline std::string get_conn_str() {return connection_string;}
  inline std::string get_db_name() {return db_name_string;}
  inline std::string get_db_coll() {return db_coll_string;}
};

// Mongo Responses

class MongoResponse: public MongoResponseInterface {
  // Internal String variables for storing response data
  int type;
  char * cval = NULL;
  std::string val;
  char err_msg[504];
  std::string err_string;

 public:
  // Build the mongo response from a C String passed back by libmongo
  inline MongoResponse(char * cstring, int resp_type) {
    cval = cstring;
    val.assign(cval);
    type = resp_type;
    err_string = "";
  }

  // Free the internal C String on delete
  inline ~MongoResponse() {
    if (cval) {
      if (type == MONGO_RESPONSE_CRT) {
        delete[] cval;
      } else {
        bson_free(cval);
      }
    }
  }

  // Retrieve the stored value for the response
  inline std::string get_value() {return val;}

  // Get the error string
  inline std::string get_err_msg() {return err_string;}

  // Set the error string
  inline void set_err_msg(char err[]) {
    strcpy(err, err_msg);
    err_string.assign(err_msg);
  }
};

// Returned from Queries in order to iterate over results
class MongoIterator: public MongoIteratorInterface {
  mongoc_cursor_t *cursor = NULL;
  MongoConnectionPool *pool = NULL;
  MongoSession *session = NULL;

 public:
  inline MongoIterator(mongoc_cursor_t *c, MongoConnectionPool *p, \
    MongoSession *s) {
    cursor = c;
    pool = p;
    session = s;
  }

  inline ~MongoIterator() {
    mongoc_cursor_destroy(cursor);
    pool->release_connection(session);
  }
  // Get the next value from the iterator.

  // Will return a Mongo Response Interface when available,
  // Otherwise will return NULL.
  MongoResponseInterface* next();
};

// Mongo Client

class MongoClient: public MongoInterface {
  // The internal mongoc client
  MongoConnectionPool *pool = NULL;

  // Initialize the client
  void initialize(const char * url, const char * db, \
    const char * collection_name, int size);

 public:
  // Switch the current collection
  void switch_collection(const char * collection_name);

  inline void switch_collection(std::string collection_name) {
    switch_collection(collection_name.c_str());
  }

  // Switch the current database
  void switch_database(const char * database_name);

  inline void switch_database(std::string database_name) {
    switch_database(database_name.c_str());
  }

  // Constructor
  inline MongoClient(const char * url, const char * db, \
    const char * collection_name) {
    initialize(url, db, collection_name, 5);
  }

  inline MongoClient(std::string url, std::string db, \
    std::string collection_name) {
    initialize(url.c_str(), db.c_str(), collection_name.c_str(), 5);
  }

  inline MongoClient(const char * url, const char * db) {
    initialize(url, db, "default", 5);
  }

  inline MongoClient(std::string url, std::string db) {
    initialize(url.c_str(), db.c_str(), "default", 5);
  }

  inline MongoClient(const char * url, const char * db, \
    const char * collection_name, int pool_size) {
    initialize(url, db, collection_name, pool_size);
  }

  inline MongoClient(std::string url, std::string db, \
    std::string collection_name, int pool_size) {
    initialize(url.c_str(), db.c_str(), collection_name.c_str(), pool_size);
  }

  inline MongoClient(const char * url, const char * db, int pool_size) {
    initialize(url, db, "default", pool_size);
  }

  inline MongoClient(std::string url, std::string db, int pool_size) {
    initialize(url.c_str(), db.c_str(), "default", pool_size);
  }

  // Destructor
  ~MongoClient();

  // CRUD Operations

  // Create JSON Document
  // Returns the document key
  MongoResponseInterface* create_document(const char * doc);

  inline MongoResponseInterface* create_document(std::string doc) {
    return create_document(doc.c_str());
  }

  inline MongoResponseInterface* create_document(const char * doc, \
    const char * collection_name) {
    switch_collection(collection_name);
    return create_document(doc);
  }

  inline MongoResponseInterface* create_document(std::string doc, \
    std::string collection_name) {
    switch_collection(collection_name);
    return create_document(doc.c_str());
  }

  // Delete a JSON Document
  void delete_document(const char * key);

  inline void delete_document(std::string key) {
    return delete_document(key.c_str());
  }

  inline void delete_document(const char * key, const char * collection_name) {
    switch_collection(collection_name);
    return delete_document(key);
  }

  inline void delete_document(std::string key, std::string collection_name) {
    return delete_document(key.c_str(), collection_name.c_str());
  }

  // Retrieve a JSON Document
  MongoResponseInterface* load_document(const char * key);

  inline MongoResponseInterface* load_document(std::string key) {
    return load_document(key.c_str());
  }

  inline MongoResponseInterface* load_document(const char * key, \
    const char * collection_name) {
    switch_collection(collection_name);
    return load_document(key);
  }

  inline MongoResponseInterface* load_document(std::string key, \
    std::string collection_name) {
    return load_document(key.c_str(), collection_name.c_str());
  }

  // Save Document
  // If the document has an '_id' field it will be updated.
  // Otherwise it will be inserted
  void save_document(const char * doc, const char * key);

  inline void save_document(std::string doc, std::string key) {
    return save_document(doc.c_str(), key.c_str());
  }

  inline void save_document(const char * doc, const char * key, \
    const char * collection_name) {
    switch_collection(collection_name);
    return save_document(doc, key);
  }

  inline void save_document(std::string doc, std::string key, \
    std::string collection_name) {
    return save_document(doc.c_str(), key.c_str(), collection_name.c_str());
  }

  // Advanced Operations

  // Queries, accept the query in JSON format
  // May also include options in JSON format
  MongoIteratorInterface* query(const char * query_str, const char * opts_str);

  inline MongoIteratorInterface* query(std::string query_str, \
    std::string opts_str) {
    return query(query_str.c_str(), opts_str.c_str());
  }

  inline MongoIteratorInterface* query(const char * query_str, \
    const char * opts_str, const char * collection_name) {
    switch_collection(collection_name);
    return query(query_str, opts_str);
  }

  inline MongoIteratorInterface* query(std::string query_str, \
    std::string opts_str, std::string collection_name) {
    switch_collection(collection_name);
    return query(query_str.c_str(), opts_str.c_str());
  }

  inline MongoIteratorInterface* query(const char * query_str) {
    return query(query_str, NULL);
  }

  inline MongoIteratorInterface* query(std::string query_str) {
    return query(query_str.c_str());
  }
};

#endif  // AOSSL_MONGO_INCLUDE_MONGO_ADMIN_H_
