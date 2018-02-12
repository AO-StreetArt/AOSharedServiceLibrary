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

#ifndef AOSSL_NEO4J_INCLUDE_NEO4J_ADMIN_H_
#define AOSSL_NEO4J_INCLUDE_NEO4J_ADMIN_H_

#include <stdio.h>
#include <errno.h>
#include <neo4j-client.h>
#include <string>
#include <exception>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <mutex>
#include "neo4j_interface.h"
#include "neo4j_result_iterator.h"
#include "neo4j_connection_pool.h"


// Neo4j Admin
class Neo4jAdmin: public Neo4jInterface {
  Neo4jConnectionPool *pool = NULL;
  void initialize(const char * conn_str, bool secure, int conn_pool_size, int pool_start_size, int pool_batch_size);

 public:
   // Initializers
   inline Neo4jAdmin(const char * conn_str, bool secure, int pool_size, \
     int pstart_size, int pbatch_size) {
     initialize(conn_str, secure, pool_size, pstart_size, pbatch_size);
   }
   inline Neo4jAdmin(std::string conn_str, bool secure, int pool_size, \
     int pstart_size, int pbatch_size) {
     initialize(conn_str.c_str(), secure, pool_size, pstart_size, pbatch_size);
   }
  inline Neo4jAdmin(const char * conn_str, bool secure, int pool_size) {
    initialize(conn_str, secure, pool_size, 0, 1);
  }
  inline Neo4jAdmin(std::string conn_str, bool secure, int pool_size) {
    initialize(conn_str.c_str(), secure, pool_size, 0, 1);
  }
  inline Neo4jAdmin(const char * conn_str, bool secure) {
    initialize(conn_str, secure, 5, 0, 1);
  }
  inline Neo4jAdmin(std::string conn_str, bool secure) {
    initialize(conn_str.c_str(), secure, 5, 0, 1);
  }
  Neo4jAdmin(const char * conn_str) {initialize(conn_str, false, 5, 0, 1);}
  Neo4jAdmin(std::string conn_str) {initialize(conn_str.c_str(), false, 5, 0, 1);}
  // Destructors
  ~Neo4jAdmin() {if (pool) {delete pool;}}
  // Query methods
  ResultsIterator* execute(const char * query);
  ResultsIterator* execute(std::string query) {return execute(query.c_str());}
  ResultsIteratorInterface* execute(const char * query, \
    std::unordered_map<std::string, \
    Neo4jQueryParameterInterface*> query_params);
  ResultsIteratorInterface* execute(std::string query, \
    std::unordered_map<std::string, \
    Neo4jQueryParameterInterface*> query_params) {
    return execute(query.c_str(), query_params);
  }
};

#endif  // AOSSL_NEO4J_INCLUDE_NEO4J_ADMIN_H_
