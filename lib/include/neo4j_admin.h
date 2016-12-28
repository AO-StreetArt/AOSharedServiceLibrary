#include <neo4j-client.h>
#include <errno.h>
#include <string>
#include <exception>
#include <vector>
#include <unordered_map>
#include <iostream>

#include "factory/neo4j_interface.h"

#ifndef AOSSL_NEO4J_ADMIN
#define AOSSL_NEO4J_ADMIN

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
class ResultsIterator: public ResultsIteratorInterface {
neo4j_result_stream_t *results;
void clear_results() {neo4j_close_results(results);}
public:

  inline ResultsIterator(neo4j_result_stream_t *result_stream) {
    results=result_stream;
    if (!results) {
      std::string err_msg (strerror(errno));
      err_msg="Failed to fetch results"+err_msg;
      throw Neo4jException(err_msg);
    }
  }
  ~ResultsIterator() {clear_results();}
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

class Neo4jQueryParameter: public Neo4jQueryParameterInterface {
bool bool_value;
std::string str_value;
int int_value;
double double_value;
int type;
public:
  Neo4jQueryParameter(bool inp_bool) {bool_value = inp_bool; type = _BOOL_TYPE;}
  Neo4jQueryParameter(std::string inp_str) {str_value = inp_str; type = _STR_TYPE;}
  Neo4jQueryParameter(const char * inp_str) {std::string new_val (inp_str); str_value = new_val; type = _STR_TYPE;}
  Neo4jQueryParameter(int inp_int) {int_value = inp_int; type = _INT_TYPE;}
  Neo4jQueryParameter(double inp_double) {double_value = inp_double; type = _FLT_TYPE;}
  int get_type() {return type;}
  bool get_boolean_value() {return bool_value;}
  std::string get_string_value() {return str_value;}
  int get_integer_value() {return int_value;}
  double get_double_value() {return double_value;}
};

class Neo4jAdmin: public Neo4jInterface {

neo4j_connection_t *connection = NULL;
neo4j_session_t *session = NULL;
void initialize(const char * conn_str, bool secure);

public:

  Neo4jAdmin(const char * conn_str, bool secure) {initialize(conn_str, secure);}
  Neo4jAdmin(std::string conn_str, bool secure) {initialize(conn_str.c_str(), secure);}
  Neo4jAdmin(const char * conn_str) {initialize(conn_str, false);}
  Neo4jAdmin(std::string conn_str) {initialize(conn_str.c_str(), false);}
  ~Neo4jAdmin() {neo4j_end_session(session);neo4j_close(connection);neo4j_client_cleanup();}
  ResultsIterator* execute(const char * query);
  ResultsIterator* execute(std::string query) {return execute(query.c_str());}
  ResultsIteratorInterface* execute(const char * query, std::unordered_map<std::string, Neo4jQueryParameterInterface*> query_params);
  ResultsIteratorInterface* execute(std::string query, std::unordered_map<std::string, Neo4jQueryParameterInterface*> query_params) {return execute(query.c_str(), query_params);}

};

#endif
