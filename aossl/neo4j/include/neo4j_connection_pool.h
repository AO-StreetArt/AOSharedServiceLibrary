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

// Connection Pooling

#include <neo4j-client.h>
#include <mutex>
#include <string>
#include <vector>
#include "aossl/core/include/slot_pool.h"

#ifndef AOSSL_NEO4J_INCLUDE_NEO4J_CONNECTION_POOL_H_
#define AOSSL_NEO4J_INCLUDE_NEO4J_CONNECTION_POOL_H_

// A struct containing the objects needed to run a query
struct Neo4jQuerySession {
  neo4j_connection_t *connection = NULL;
  int index = -1;
};

// A Connection pool to ensure thread safety
class Neo4jConnectionPool {
  // A pool of neo4j connections
  std::vector<Neo4jQuerySession> connections;
  // Slot pool to manage the internal slots
  AOSSL::SlotPool *slot_pool = NULL;
  // Internal integers
  int connection_limit = 1;
  int start_connections = 0;
  int current_connection = -1;
  int current_max_connection = 1;
  int connection_creation_batch = 1;
  // Secure connection with Neo4j?
  bool secure = false;
  // The string holding connection information
  std::string connection_string;
  // Internal Mutex to ensure that new connections are created one at a time
  std::mutex create_conn_mutex;

  // Start our initial connections
  inline void init_connections(const char * conn_str, bool secure) {
    for (int i = 0; i < start_connections; i++) {
      Neo4jQuerySession qs;

      // Create the connection
      if (!secure) {
        qs.connection = neo4j_connect(conn_str, NULL, NEO4J_INSECURE);
      } else {
        qs.connection = neo4j_connect(conn_str, NULL, NEO4J_CONNECT_DEFAULT);
      }

      // Check if the connection was successful
      if (!(qs.connection)) {
        const char * err_string = strerror(errno);
        std::string err_msg(err_string);
        err_msg = "Error establishing connection: " + err_msg;
        throw Neo4jException(err_msg);
      }

      connections.push_back(qs);
    }
    current_max_connection = start_connections;
  }

  // Base Startup Routine
  inline void init(int size, const char * conn_str, bool secure_connect) {
    connection_limit = size;
    connection_string.assign(conn_str);
    secure = secure_connect;
    slot_pool = new AOSSL::SlotPool(connection_limit);
    neo4j_client_init();
    init_connections(conn_str, secure);
  }

 public:
  // Constructors
  Neo4jConnectionPool(int size, const char * conn_str, bool secure_connect) \
    {init(size, conn_str, secure_connect);}
  inline Neo4jConnectionPool(int size, const char * conn_str, \
    bool secure_connect, int start_conns) {
    start_connections = start_conns;
    init(size, conn_str, secure_connect);
  }
  inline Neo4jConnectionPool(int size, const char * conn_str, \
    bool secure_connect, int start_conns, int batch_size) {
    connection_creation_batch = batch_size;
    start_connections = start_conns;
    init(size, conn_str, secure_connect);
  }

  // Destructor
  inline ~Neo4jConnectionPool() {
    for (int i = 0; i < current_max_connection; i++) {
      // Release the connection
      neo4j_close(connections[i].connection);
    }
    neo4j_client_cleanup();
    delete slot_pool;
  }

  // Get a connection to use
  inline Neo4jQuerySession* get_connection() {
    // Find the next available connection slot
    // If none are available, wait until one is freed
    int current_connection = slot_pool->find_next_slot();

    // Create new connections if necessary
    if (current_connection > current_max_connection) {
      // Get the creation mutex to ensure that
      // only 1 thread creates new connections
      std::lock_guard<std::mutex> lock(create_conn_mutex);
      // Double check and see if any other threads
      // built connections since we asked for the mutex
      if (current_connection > current_max_connection) {
        for (
          int i = current_max_connection;
          i < current_max_connection + connection_creation_batch;
          i++
        ) {
          Neo4jQuerySession qs;
          if (!secure) {
            qs.connection = neo4j_connect(connection_string.c_str(), \
              NULL, NEO4J_INSECURE);
          } else {
            qs.connection = neo4j_connect(connection_string.c_str(), \
              NULL, NEO4J_CONNECT_DEFAULT);
          }

          // Check if the connection was successful
          if (!(qs.connection)) {
            const char * err_string = strerror(errno);
            std::string err_msg(err_string);
            err_msg = "Error establishing connection: " + err_msg;
            throw Neo4jException(err_msg);
          }

          connections.push_back(qs);
        }
        current_max_connection = current_max_connection + \
          connection_creation_batch;
      }
    }

    // Pack the connection & session into the return object
    Neo4jQuerySession *s = new Neo4jQuerySession;
    s->connection = connections[current_connection].connection;
    s->index = current_connection;

    // Return the latest connection information
    // When we leave the method, the mutex lock is released automatically
    return s;
  }

  // Release a connection after use
  inline void release_connection(Neo4jQuerySession *conn) {
    // Release the slot
    slot_pool->release_slot(conn->index);
    // Delete the query session pointer, but not it's contents
    delete conn;
  }
};

#endif  // AOSSL_NEO4J_INCLUDE_NEO4J_CONNECTION_POOL_H_
