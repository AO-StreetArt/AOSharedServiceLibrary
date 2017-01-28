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

#include <neo4j-client.h>
#include <errno.h>
#include <string>
#include <exception>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <mutex>

#include "neo4j_interface.h"

#ifndef AOSSL_NEO4J_ADMIN
#define AOSSL_NEO4J_ADMIN

//-------------------------------------------------------------------
////------------------Connection Pooling-----------------------------
////-----------------------------------------------------------------

//A struct containing the objects needed to run a query
struct Neo4jQuerySession {
  neo4j_connection_t *connection = NULL;
  neo4j_session_t *session = NULL;
  int index = -1;
};

//A Connection pool to ensure thread safety
class Neo4jConnectionPool {
  //A pool of neo4j connections
  std::vector<Neo4jQuerySession> connections;
  //Array of ints (0/1) which determine which connections are open vs closed
  int *slots;
  //Internal integers
  int connection_limit = 1;
  int start_connections = 1;
  int current_connection = -1;
  int current_max_connection = 1;
  int connection_creation_batch = 1;
  bool secure = false;
  std::string connection_string;
  std::mutex get_conn_mutex;
  void init_slots();
  void init_connections(const char * conn_str, bool secure);
public:
  Neo4jConnectionPool(int size, const char * conn_str, bool secure_connect) {connection_limit=size;std::string con_str (conn_str);connection_string=con_str;secure=secure_connect;init_slots();neo4j_client_init();init_connections(conn_str, secure);}
  Neo4jConnectionPool(int size, const char * conn_str, bool secure_connect, int start_conns) {std::string con_str (conn_str);connection_string=con_str;secure=secure_connect;init_slots();start_connections=start_conns;connection_limit=size;init_connections(conn_str, secure);}
  Neo4jConnectionPool(int size, const char * conn_str, bool secure_connect, int start_conns, int batch_size) {std::string con_str (conn_str);connection_string=con_str;secure=secure_connect;init_slots();start_connections=start_conns;connection_limit=size;connection_creation_batch=batch_size;init_connections(conn_str, secure);}
  ~Neo4jConnectionPool();
  Neo4jQuerySession* get_connection();
  void release_connection(Neo4jQuerySession *conn);
};

//-------------------------------------------------------------------
////---------------------Query Results-------------------------------
////-----------------------------------------------------------------

typedef neo4j_value_t (*ValueGenerationFunction)(neo4j_value_t);
typedef neo4j_value_t (*KeyGenerationFunction)(neo4j_value_t, neo4j_value_t);
typedef neo4j_value_t (*IndexGenerationFunction)(neo4j_value_t, unsigned int);

class DbList: public DbListInterface {
  neo4j_result_t *result = NULL;
  int index;
  int path_index;
  ValueGenerationFunction list_function;
  KeyGenerationFunction map_function = NULL;
  std::string map_key;
  neo4j_value_t get_list();
  neo4j_value_t get_list_value(unsigned int ind);
public:
  DbList(neo4j_result_t *r, unsigned int ind, ValueGenerationFunction lf) {result=r;index=ind;list_function=lf;}
  DbList(neo4j_result_t *r, unsigned int ind, ValueGenerationFunction lf, unsigned int pindex) {result=r;index=ind;list_function=lf;path_index=pindex;}
  DbList(neo4j_result_t *r, unsigned int ind, ValueGenerationFunction lf, unsigned int pindex, KeyGenerationFunction mf, std::string mf_key) {result=r;index=ind;list_function=lf;path_index=pindex;map_function=mf;map_key=mf_key;}
  ~DbList() {}
  bool get_bool_element(unsigned int ind);
  int get_int_element(unsigned int ind);
  double get_float_element(unsigned int ind);
  std::string get_string_element(unsigned int ind, int char_buffer_size);
  std::string get_string_element(unsigned int ind) {return get_string_element(ind, 128);}
  std::string to_string();
  unsigned int size();
};

class DbMap: public DbMapInterface {
  neo4j_result_t *result = NULL;
  int index;
  int path_index;
  ValueGenerationFunction map_function;
  std::vector<KeyGenerationFunction> map_functions;
  std::vector<std::string> keys;
  neo4j_value_t get_map();
  neo4j_value_t get_map_value(std::string key);
public:
  DbMap(neo4j_result_t *r, int ind, ValueGenerationFunction mf) {result=r;index=ind;map_function =mf;}
  DbMap(neo4j_result_t *r, int ind, ValueGenerationFunction mf, int pindex) {result=r;index=ind;map_function =mf;path_index=pindex;}
  DbMap(neo4j_result_t *r, int ind, std::vector<std::string> key_list, ValueGenerationFunction mf, std::vector<KeyGenerationFunction> mfs);
  DbMap(neo4j_result_t *r, int ind, std::vector<std::string> key_list, ValueGenerationFunction mf, std::vector<KeyGenerationFunction> mfs, int pindex);
  ~DbMap() {}
  unsigned int size();
  bool element_exists(std::string key);
  std::string get_string_element(std::string key, int char_buffer_size);
  std::string get_string_element(std::string key) {return get_string_element(key, 128);}
  bool get_bool_element(std::string key);
  int get_int_element(std::string key);
  double get_float_element(std::string key);
  DbListInterface* get_list_element(std::string key);
  std::string to_string();
};

//Represents a single node, edge, or path
class DbObject: public DbObjectInterface {
  neo4j_result_t *result = NULL;
  int index;
  int path_index;
  neo4j_value_t get_value();
  bool is_instance_of(neo4j_type_t type);
public:
  DbObject(neo4j_result_t *r, int ind) {result=r;index=ind;path_index=-1;}
  DbObject(neo4j_result_t *r, int ind, int ind2) {result=r;index=ind;path_index=ind2;}
  ~DbObject() {}
  bool is_node() {return is_instance_of(NEO4J_NODE);}
  bool is_edge() {return is_instance_of(NEO4J_RELATIONSHIP);}
  bool is_path() {return is_instance_of(NEO4J_PATH);}
  std::string to_string();
  DbMapInterface* properties();
  DbListInterface* labels();
  std::string type();
  bool forward();
  neo4j_value_t get_identity();
  bool is_equal_to(DbObject& comp_obj) {return neo4j_eq(get_identity(), comp_obj.get_identity());}
  unsigned int size();
  DbObjectInterface* get_path_element(int path_index) {return new DbObject(result, index, path_index);}
};

// Represents a single query Result, returned by the iterator
// Consists of a set of nodes and edges
class ResultTree: public ResultTreeInterface {
  neo4j_result_t *result = NULL;
public:
  ResultTree(neo4j_result_t *r) {result=r;}
  ~ResultTree() {}
  DbObjectInterface* get(int index);
};

// Results Iterator, accepts a result stream as an input,
// Returns Results Trees for each entry that matches in the query
// Deletes it when finished
// Also responsible for releasing the database connection with the pool after usage
class ResultsIterator: public ResultsIteratorInterface {
  neo4j_result_stream_t *results;
  Neo4jQuerySession *s;
  Neo4jConnectionPool *p;
  void clear_results() {neo4j_close_results(results);}
public:

  inline ResultsIterator(neo4j_result_stream_t *result_stream, Neo4jQuerySession *session, Neo4jConnectionPool *pool) {
    results=result_stream;
    if (!results) {
      std::string err_msg (strerror(errno));
      err_msg="Failed to fetch results"+err_msg;
      p->release_connection(s);
      throw Neo4jException(err_msg);
    }
    s=session;
    p=pool;
  }
  ~ResultsIterator() {clear_results();p->release_connection(s);}
  void clear() {clear_results();}
  bool empty() {if (!results) {return true;} return false;}
  unsigned int length() {return neo4j_nfields(results);}
  ResultTreeInterface* next();
};

//-------------------------------------------------------------------
////----------------------Neo4j Admin--------------------------------
////-----------------------------------------------------------------

const int _BOOL_TYPE = 0;
const int _STR_TYPE = 1;
const int _INT_TYPE = 2;
const int _FLT_TYPE = 3;

//A Query parameter to be inserted into cypher queries
class Neo4jQueryParameter: public Neo4jQueryParameterInterface {
  bool bool_value;
  std::string str_value;
  int int_value;
  double double_value;
  int type;
  bool is_list;
  std::vector<bool> bool_values;
  std::vector<std::string> str_values;
  std::vector<int> int_values;
  std::vector<double> double_values;

public:
  Neo4jQueryParameter() {is_list=true;type=-1;}
  Neo4jQueryParameter(bool inp_bool) {bool_value = inp_bool; type = _BOOL_TYPE;is_list=false;}
  Neo4jQueryParameter(std::string inp_str) {str_value = inp_str; type = _STR_TYPE;is_list=false;}
  Neo4jQueryParameter(const char * inp_str) {std::string new_val (inp_str); str_value = new_val; type = _STR_TYPE;is_list=false;}
  Neo4jQueryParameter(int inp_int) {int_value = inp_int; type = _INT_TYPE;is_list=false;}
  Neo4jQueryParameter(double inp_double) {double_value = inp_double; type = _FLT_TYPE;is_list=false;}
  int get_type() {return type;}
  bool get_boolean_value() {if (!is_list) {return bool_value;} else {throw Neo4jException("Attempting to retrieve single value from array element");}}
  bool get_boolean_value(int index) {if (is_list) {return bool_values[index];} else {throw Neo4jException("Attempting to retrieve indexed value from single element");}}
  std::string get_string_value() {if (!is_list) {return str_value;} else {throw Neo4jException("Attempting to retrieve single value from array element");}}
  std::string get_string_value(int index) {if (is_list) {return str_values[index];} else {throw Neo4jException("Attempting to retrieve indexed value from single element");}}
  int get_integer_value() {if (!is_list) {return int_value;} else {throw Neo4jException("Attempting to retrieve full single from array element");}}
  int get_integer_value(int index) {if (is_list) {return int_values[index];} else {throw Neo4jException("Attempting to retrieve indexed value from single element");}}
  double get_double_value() {if (!is_list) {return double_value;} else {throw Neo4jException("Attempting to retrieve single value from array element");}}
  double get_double_value(int index) {if (is_list) {return double_values[index];} else {throw Neo4jException("Attempting to retrieve indexed value from single element");}}
  bool is_array() {return is_list;}
  unsigned int size();
  void add_value(bool new_val);
  void add_value(std::string new_val);
  void add_value(const char * new_val);
  void add_value(int new_val);
  void add_value(float new_val);
};

class Neo4jAdmin: public Neo4jInterface {
  Neo4jConnectionPool *pool = NULL;
  void initialize(const char * conn_str, bool secure, int conn_pool_size);
public:

  Neo4jAdmin(const char * conn_str, bool secure, int pool_size) {initialize(conn_str, secure, pool_size);}
  Neo4jAdmin(std::string conn_str, bool secure, int pool_size) {initialize(conn_str.c_str(), secure, pool_size);}
  Neo4jAdmin(const char * conn_str, bool secure) {initialize(conn_str, secure, 5);}
  Neo4jAdmin(std::string conn_str, bool secure) {initialize(conn_str.c_str(), secure, 5);}
  Neo4jAdmin(const char * conn_str) {initialize(conn_str, false, 5);}
  Neo4jAdmin(std::string conn_str) {initialize(conn_str.c_str(), false, 5);}
  ~Neo4jAdmin() {if (pool) {delete pool;}}
  ResultsIterator* execute(const char * query);
  ResultsIterator* execute(std::string query) {return execute(query.c_str());}
  ResultsIteratorInterface* execute(const char * query, std::unordered_map<std::string, Neo4jQueryParameterInterface*> query_params);
  ResultsIteratorInterface* execute(std::string query, std::unordered_map<std::string, Neo4jQueryParameterInterface*> query_params) {return execute(query.c_str(), query_params);}

};

#endif
