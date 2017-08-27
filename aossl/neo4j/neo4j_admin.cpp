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

#include "include/neo4j_admin.h"

  // Start the admin
void Neo4jAdmin::initialize(const char * conn_str, bool secure, int pool_size) {
  // Initialize the Neo4j Database pool
  pool = new Neo4jConnectionPool(pool_size, conn_str, secure, 1, 1);
}

// Execute a query and return the results in an iterator
ResultsIterator* Neo4jAdmin::execute(const char * query) {
  Neo4jQuerySession *qs = pool->get_connection();
  // Execute the query
  neo4j_result_stream_t *res = neo4j_run(qs->session, query, neo4j_null);

  // Check for a failure
  int failure_check = neo4j_check_failure(res);
  if (failure_check != 0) {
    const struct neo4j_failure_details *fd;
    fd = neo4j_failure_details(res);
    pool->release_connection(qs);
    throw Neo4jException(fd->description, res);
  }

  // Return a results iterator for results access
  return new ResultsIterator(res, qs, pool);
}

// Insert a map of parameters into a query,
// Execute the query and return the results in an iterator
ResultsIteratorInterface* Neo4jAdmin::execute(const char * query, \
  std::unordered_map<std::string, Neo4jQueryParameterInterface*> query_params) {
  // Get lists of keys and values for query parameters
  std::vector<std::string> keys;
  keys.reserve(query_params.size());


  std::vector<Neo4jQueryParameterInterface*> vals;
  vals.reserve(query_params.size());

  for (auto kv : query_params) {
    keys.push_back(kv.first);
    vals.push_back(kv.second);
  }

  // Create an array of neo4j map entries
  neo4j_map_entry_t map_entries[keys.size()];
  neo4j_value_t map_values[vals.size()];

  for (unsigned int i = 0; i < keys.size(); i++) {
    Neo4jQueryParameterInterface* val = vals[i];
    int val_type = val->get_type();
    // We have single value parameter, and just need to determine the type
    if (val_type == _BOOL_TYPE) {
      map_values[i] = neo4j_bool(val->get_boolean_value());
      map_entries[i] = neo4j_map_entry(keys[i].c_str(), map_values[i]);
    } else if (val_type == _STR_TYPE) {
      const char * val_str = val->get_cstring_value();
      if (!val_str) {
        throw Neo4jException("query parameters must have length > 1.");
      } else {
        if (keys[i].empty()) {
          throw Neo4jException("query parameter keys must have length > 1.");
        }
        map_values[i] = neo4j_string(val->get_cstring_value());
        map_entries[i] = neo4j_map_entry(keys[i].c_str(), map_values[i]);
      }
    } else if (val_type == _INT_TYPE) {
      map_values[i] = neo4j_int(val->get_integer_value());
      map_entries[i] = neo4j_map_entry(keys[i].c_str(), map_values[i]);
    } else if (val_type == _FLT_TYPE) {
      map_values[i] = neo4j_float(val->get_double_value());
      map_entries[i] = neo4j_map_entry(keys[i].c_str(), map_values[i]);
    }
  }

  // Create a neo4j value of the map
  neo4j_value_t param_map = neo4j_map(map_entries, keys.size());

  // Retrieve a database connection from the pool
  Neo4jQuerySession *qs = pool->get_connection();

  // Execute the query
  neo4j_result_stream_t *res = neo4j_run(qs->session, query, param_map);

  // Check for a failure
  int failure_check = neo4j_check_failure(res);

  // If a failure is found
  if (failure_check != 0) {
    // Release the connection from the pool
    pool->release_connection(qs);

    // If our failure was related to the statement, then throw
    // a related exception
    if (failure_check == NEO4J_STATEMENT_EVALUATION_FAILED) {
      const struct neo4j_failure_details *fd;
      fd = neo4j_failure_details(res);
      if (fd && fd->description) {
        throw Neo4jException(fd->description, res);
      }
    }

    // If we weren't able to retrieve failure details, throw a general exception
    throw Neo4jException("Unknown error encountered running query", res);
  }

  // Return a results iterator for results access
  return new ResultsIterator(res, qs, pool);
}
