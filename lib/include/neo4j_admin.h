#include <neo4j-client.h>
#include <errno.h>
#include <string>
#include <exception>
#include <vector>

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
ValueGenerationFunction list_function;
std::vector<KeyGenerationFunction> map_functions;
std::vector<IndexGenerationFunction> list_functions;
std::vector<std::string> keys;
std::vector<unsigned int> indices;
neo4j_value_t get_list();
void initialize(neo4j_result_t *r, unsigned int ind, std::vector<std::string> key_list, ValueGenerationFunction mf, std::vector<KeyGenerationFunction> mfs);
neo4j_value_t get_list_value(unsigned int ind);
public:
  DbList(neo4j_result_t *r, unsigned int ind, ValueGenerationFunction lf) {result=r;index=ind;list_function=lf;}
  DbList(neo4j_result_t *r, unsigned int ind, std::vector<std::string> key_list, ValueGenerationFunction mf, std::vector<KeyGenerationFunction> mfs);
  DbList(neo4j_result_t *r, unsigned int ind, std::vector<std::string> key_list, std::vector<unsigned int> inds, ValueGenerationFunction mf, std::vector<KeyGenerationFunction> mfs, std::vector<IndexGenerationFunction> lfs);
  ~DbList() {}
  DbListInterface* get_list_element(unsigned int ind);
  bool get_bool_element(unsigned int ind);
  int get_int_element(unsigned int ind);
  float get_float_element(unsigned int ind);
  std::string get_string_element(unsigned int ind, int char_buffer_size);
  std::string get_string_element(unsigned int ind) {return get_string_element(ind, 128);}
  std::string to_string();
  unsigned int size();
};

class DbMap: public DbMapInterface {
neo4j_result_t *result = NULL;
int index;
ValueGenerationFunction map_function;
std::vector<KeyGenerationFunction> map_functions;
std::vector<std::string> keys;
neo4j_value_t get_map();
neo4j_value_t get_map_value(std::string key);
public:
  DbMap(neo4j_result_t *r, int ind, ValueGenerationFunction mf) {result=r;index=ind;map_function =mf;}
  DbMap(neo4j_result_t *r, int ind, std::vector<std::string> key_list, ValueGenerationFunction mf, std::vector<KeyGenerationFunction> mfs);
  ~DbMap() {}
  unsigned int size();
  std::string get_string_element(std::string key, int char_buffer_size);
  std::string get_string_element(std::string key) {return get_string_element(key, 128);}
  bool get_bool_element(std::string key);
  int get_int_element(std::string key);
  float get_float_element(std::string key);
  DbMapInterface* get_map_element(std::string key);
  DbListInterface* get_list_element(std::string key);
  std::string to_string();
};

//Represents a single node or edge
class DbObject: public DbObjectInterface {
neo4j_result_t *result = NULL;
int index;
neo4j_value_t get_value();
bool is_instance_of(neo4j_type_t type);
public:
  DbObject(neo4j_result_t *r, int ind) {result=r;index=ind;}
  ~DbObject() {}
  bool is_node() {return is_instance_of(NEO4J_NODE);}
  bool is_edge() {return is_instance_of(NEO4J_RELATIONSHIP);}
  std::string to_string();
  DbMapInterface* properties();
  DbListInterface* labels();
  std::string type();
  neo4j_value_t get_identity();
  bool is_equal_to(DbObject& comp_obj) {return neo4j_eq(get_identity(), comp_obj.get_identity());}
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

  ResultsIterator(neo4j_result_stream_t *result_stream);
  ~ResultsIterator() {clear_results();}
  void clear() {clear_results();}
  bool empty() {if (!results) {return true;} return false;}
  unsigned int length() {return neo4j_nfields(results);}
  ResultTreeInterface* next();
};

//-------------------------------------------------------------------
////----------------------Neo4j Admin--------------------------------
////-----------------------------------------------------------------

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
};

#endif
