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

#include "include/mongo_admin.h"

MongoResponseInterface* MongoIterator::next() {
  // Return the next result from the internal cursor
  const bson_t *doc = NULL;
  mongoc_cursor_next(cursor, &doc);
  bson_error_t error;

  // If a document was returned from the cursor, then wrap it in a Mongo resp
  if (doc) {
    MongoResponse *resp = \
      new MongoResponse(bson_as_json(doc, NULL), MONGO_RESPONSE_OTHER);
    if (mongoc_cursor_error(cursor, &error)) resp->set_err_msg(error.message);
    return resp;
  } else {
    return NULL;
  }
}

void MongoClient::initialize(const char * url, const char * db, \
  const char * collection_name, int size) {
  std::string url_str(url);
  std::string db_str(db);
  std::string col_str(collection_name);
  pool = new MongoConnectionPool(size, url_str, db_str, col_str, 1, 1);
}

MongoClient::~MongoClient() {
  if (pool) {delete pool;}
}

MongoResponseInterface* MongoClient::create_document(const char * document) {
  bson_t *doc = NULL;
  bson_error_t error;
  bson_oid_t oid;
  char * c_str = new char[25];

  // Perform the conversion
  doc = bson_new_from_json((const uint8_t *)document, -1, &error);

  // Check if the conversion succeeded
  bool success = false;
  if (!doc) {
    std::string err1 = "Error Converting JSON: ";
    std::string err2(error.message);
    throw MongoException(err1+err2);
  } else {
    // Append the key
    bson_oid_init(&oid, NULL);
    BSON_APPEND_OID(doc, "_id", &oid);
    bson_oid_to_string(&oid, c_str);

    // Retrieve a database connection from the pool
    MongoSession *ms = pool->get_connection();

    // Actually insert the document
    success = mongoc_collection_insert(ms->collection, MONGOC_INSERT_NONE, \
      doc, NULL, &error);

    // Release the database connection
    pool->release_connection(ms);

    // Free the BSON Document
    bson_destroy(doc);

    if (!success) {
      delete[] c_str;
      std::string err1 = "Error Creating Document: ";
      std::string err2(error.message);
      throw MongoException(err1+err2);
    }
  }

  return new MongoResponse(c_str, MONGO_RESPONSE_CRT);
}

void MongoClient::save_document(const char * document, const char * key) {
  if (key && key[0] != '\0') {
    bson_t *doc = NULL;
    bson_t *selector = NULL;
    bson_error_t error;
    bson_oid_t oid;

    // Perform the conversion
    doc = bson_new_from_json((const uint8_t *)document, -1, &error);

    // Check if the conversion succeeded
    bool success = false;
    if (!doc) {
      std::string err1 = "Error Converting JSON: ";
      std::string err2(error.message);
      throw MongoException(err1+err2);
    } else {
      // Copy our key into a character array in a memory safe manner
      // Mongo OID will be returned as 24 bit hex-encoded string
      char key_str[25];
      strncpy(key_str, key, 25);
      key_str[24] = '\0';

      // Append the key
      selector = bson_new();
      bson_oid_init_from_string(&oid, key_str);
      BSON_APPEND_OID(selector, "_id", &oid);

      // Retrieve a database connection from the pool
      MongoSession *ms = pool->get_connection();

      // Actually insert the document
      success = mongoc_collection_update(ms->collection, MONGOC_UPDATE_NONE, \
        selector, doc, NULL, &error);

      // Release the database connection
      pool->release_connection(ms);

      bson_destroy(doc);
      bson_destroy(selector);

      if (!success) {
        std::string err1 = "Error Saving Document: ";
        std::string err2(error.message);
        throw MongoException(err1+err2);
      }
    }
  } else {
    throw MongoException("No Key Passed to Delete Document Method");
  }
}

void MongoClient::delete_document(const char * key) {
  if (key && key[0] != '\0') {
    // Setup necessary variables
    bson_t *doc = NULL;
    bson_error_t error;
    bson_oid_t oid;

    // Copy our key into a character array in a memory safe manner
    // Mongo OID will be returned as 24 bit hex-encoded string
    char key_str[25];
    strncpy(key_str, key, 25);
    key_str[24] = '\0';

    // Write a new bson query with the key provided
    doc = bson_new();
    bson_oid_init_from_string(&oid, key_str);
    BSON_APPEND_OID(doc, "_id", &oid);

    // Retrieve a database connection from the pool
    MongoSession *ms = pool->get_connection();

    // Execute the query
    bool success = mongoc_collection_remove(ms->collection, \
      MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error);

    // Release the database connection
    pool->release_connection(ms);

    bson_destroy(doc);

    if (!success) {
      std::string err1 = "Error Deleting Document: ";
      std::string err2(error.message);
      throw MongoException(err1+err2);
    }
  } else {
    throw MongoException("No Key Passed to Delete Document Method");
  }
}

void MongoClient::switch_collection(const char * collection_name) {
  std::string coll_cstr(collection_name);
  pool->switch_collection(coll_cstr);
}

void MongoClient::switch_database(const char * database_name) {
  std::string db_cstr(database_name);
  pool->switch_database(db_cstr);
}

MongoResponseInterface* MongoClient::load_document(const char * key) {
  if (key && key[0] != '\0') {
    // Setup necessary variables
    bson_t *query = NULL;
    bson_oid_t oid;
    mongoc_cursor_t *cursor = NULL;
    MongoResponseInterface *ret_val = NULL;

    // Copy our key into a character array in a memory safe manner
    // Mongo OID will be returned as 24 bit hex-encoded string
    char key_str[25];
    strncpy(key_str, key, 25);
    key_str[24] = '\0';

    // Write a new bson query with the key provided
    query = bson_new();
    bson_oid_init_from_string(&oid, key_str);
    BSON_APPEND_OID(query, "_id", &oid);

    // Retrieve a database connection from the pool
    MongoSession *ms = pool->get_connection();

    // Execute the query on the key
    cursor = \
      mongoc_collection_find_with_opts(ms->collection, query, NULL, NULL);

    // Pull the first result from the query
    // In this special case, we will only ever get 1 value returned
    // we are only querying on key, so Mongo will guarantee these are unique.

    // The iterator will handle closing the cursor and the DB Connection
    if (cursor) {
      MongoIteratorInterface *iter = new MongoIterator(cursor, pool, ms);
      // The response interface will be returned to the user
      ret_val = iter->next();
      // Cleanup
      delete iter;
    } else {
      pool->release_connection(ms);
    }
    bson_destroy(query);
    return ret_val;
  } else {
    throw MongoException("No Key Passed to Load Document Method");
  }
}

MongoIteratorInterface* MongoClient::query(const char * query_str, \
  const char * opts_str) {
  if (query_str && query_str[0] != '\0') {
    // Setup necessary variables
    bson_t *q = NULL;
    bson_t *o = NULL;
    bson_error_t error;
    mongoc_cursor_t *cursor = NULL;

    // Write a new bson query with the key provided
    q = bson_new_from_json((const uint8_t *)query_str, -1, &error);

    if (!q) {throw MongoException(error.message);}

    if (opts_str) {
      o = bson_new_from_json((const uint8_t *)query_str, -1, &error);
      if (!o) {throw MongoException(error.message);}
    }

    // Retrieve a database connection from the pool
    MongoSession *ms = pool->get_connection();

    cursor = mongoc_collection_find_with_opts(ms->collection, q, o, NULL);

    MongoIteratorInterface *iter = NULL;
    if (cursor) {
      iter = new MongoIterator(cursor, pool, ms);
    }

    if (o) {
      bson_destroy(o);
    }
    bson_destroy(q);

    return iter;
  } else {
    throw MongoException("No Query Passed to Query Method");
  }
}

// Connection Pool

void MongoConnectionPool::init_slots() {
  slots = new int[connection_limit];
  for (int i=0; i < connection_limit; i++) {
    slots[i] = 0;
  }
}

// Initialize the connections required to start the connection pool
void MongoConnectionPool::init_connections(std::string conn_str, \
  std::string db, std::string coll) {
  connection_string = conn_str;
  db_name_string = db;
  db_coll_string = coll;

  // Initialize the Mongo C-Driver
  mongoc_init();

  const char * conn_cstr = conn_str.c_str();
  for (int i = 0; i < start_connections; i++) {
    MongoSession ms;

    // Create the connection
    ms.connection = mongoc_client_new(conn_cstr);
    connections.push_back(ms);
  }
  current_max_connection = start_connections;
}

MongoConnectionPool::~MongoConnectionPool() {
  for (int i = 0; i < current_max_connection; i++) {
    // Release the connection
    mongoc_client_destroy(connections[i].connection);
  }
  mongoc_cleanup();
  delete[] slots;
}

MongoSession* MongoConnectionPool::get_connection() {
  // Get the mutex to ensure we get a unique connection
  std::lock_guard<std::mutex> lock(get_conn_mutex);

  // Find the next available connection slot
  // If none are available, wait until one is freed
  bool connection_found = false;
  while (!connection_found) {
    for (int j = 0; j < connection_limit; j++) {
      if (slots[j] == 0) {
        // slot is available
        current_connection = j;
        connection_found = true;
        break;
      }
    }
  }

  // Create new connections if necessary
  if (current_connection > current_max_connection) {
    const char * conn_cstr = connection_string.c_str();
    for (
      int i = current_max_connection;
      i < current_max_connection + connection_creation_batch;
      i++
    ) {
      MongoSession ms;

      // Create the connection
      ms.connection = mongoc_client_new(conn_cstr);
      connections.push_back(ms);
    }
    current_max_connection = current_max_connection + connection_creation_batch;
  }

  // Pack the connection & collection into the return object
  MongoSession *s = new MongoSession;
  s->connection = connections[current_connection].connection;
  s->collection = mongoc_client_get_collection( \
    connections[current_connection].connection, db_name_string.c_str(), \
    db_coll_string.c_str());
  s->index = current_connection;

  // Reserve the slot
  slots[current_connection] = 1;

  // Return the latest connection information
  // When we leave the method, the mutex lock is released automatically
  return s;
}

// Release a connection for re-use
void MongoConnectionPool::release_connection(MongoSession *conn) {
  // Release the slot
  slots[conn->index] = 0;
  // Release the collection
  mongoc_collection_destroy(conn->collection);
  // Delete the query session pointer, but not it's contents
  delete conn;
}
