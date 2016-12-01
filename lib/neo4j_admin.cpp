#include "include/neo4j_admin.h"

//-----------------------------------------------------------------
//--------------------Results Iterator-----------------------------
//-----------------------------------------------------------------

//Constructor
ResultsIterator::ResultsIterator(neo4j_result_stream_t *rs) {
results=rs;
if (!results) {
  std::string err_msg (strerror(errno));
  err_msg="Failed to fetch results"+err_msg;
  throw Neo4jException(err_msg);
}
}

//Get the next result from the iterator
ResultTreeInterface* ResultsIterator::next() {
neo4j_result_t *res = neo4j_fetch_next(results);
return new ResultTree (res);
}

//-----------------------------------------------------------------
//----------------------Result Tree--------------------------------
//-----------------------------------------------------------------

//Get a DB Object
DbObjectInterface* ResultTree::get(int index) {
  return new DbObject (result, index);
}

//-----------------------------------------------------------------
//------------------------DB Map-----------------------------------
//-----------------------------------------------------------------

//Constructor
 DbMap::DbMap(neo4j_result_t *r, int ind, std::vector<std::string> key_list, ValueGenerationFunction mf, std::vector<KeyGenerationFunction> mfs) {
  result=r;
  index=ind;
  map_function = mf;
  for (int i = 0; i < mfs.size(); i++) {
    map_functions.push_back(mfs[i]);
  }
  for (int j = 0; j < key_list.size(); j++) {
    keys.push_back(key_list[j]);
  }
}

//Get the value for this node/edge
neo4j_value_t DbMap::get_map() {
if (!result) {
  return neo4j_null;
}
else {
  neo4j_value_t db_obj = neo4j_result_field(result, index);
  neo4j_value_t map = (*(map_function))(db_obj);
  for (int i = 0; i < map_functions.size(); i++) {
    neo4j_value_t key_str = neo4j_string(keys[i].c_str());
    map = (*(map_functions[i]))(map, key_str);
  }
  return map;
}
}

//Get a value out of the map
neo4j_value_t DbMap::get_map_value(std::string key) {
neo4j_value_t map = get_map();
return neo4j_map_get(map, key.c_str());
}

//Get the number of elements in the map
unsigned int DbMap::size() {

//Retrieve the map
neo4j_value_t map = get_map();

//Return the map size
return neo4j_map_size(map);
}

//Get an element out of the map

bool DbMap::get_bool_element(std::string key) {

//Get the value from the map
neo4j_value_t element = get_map_value(key);

//Convert the value to a boolean
return neo4j_bool_value(element);

}

int DbMap::get_int_element(std::string key) {

//Get the value from the map
neo4j_value_t element = get_map_value(key);

//Convert the value to an int
return neo4j_int_value(element);

}

float DbMap::get_float_element(std::string key) {

//Get the value from the map
neo4j_value_t element = get_map_value(key);

//Convert the value to a float
return neo4j_float_value(element);

}

std::string DbMap::get_string_element(std::string key, int char_buffer_size) {

//Get the value from the map
neo4j_value_t element = get_map_value(key);

//Convert the value to a string
char db_cstr[char_buffer_size];
neo4j_string_value(element, db_cstr, char_buffer_size);
std::string db_str (db_cstr);
return db_str;
}

DbMapInterface* DbMap::get_map_element(std::string key) {
  keys.push_back(key);
  map_functions.push_back(neo4j_map_kget);
  return new DbMap (result, index, keys, map_function, map_functions);
}

DbListInterface* DbMap::get_list_element(std::string key) {
  keys.push_back(key);
  map_functions.push_back(neo4j_map_kget);
  return new DbList (result, index, keys, map_function, map_functions);
}

std::string DbMap::to_string() {
neo4j_value_t value = get_map();
if (neo4j_eq(neo4j_null, value)) {
  return "";
}
char buf[128];
neo4j_tostring(value, buf, 128);
std::string ret_val (buf);
return ret_val;
}

//-----------------------------------------------------------------
//------------------------DB List----------------------------------
//-----------------------------------------------------------------

void DbList::initialize(neo4j_result_t *r, unsigned int ind, std::vector<std::string> key_list, ValueGenerationFunction mf, std::vector<KeyGenerationFunction> mfs) {
  result=r;
  index=ind;
  list_function = mf;
  for (int i = 0; i < mfs.size(); i++) {
    map_functions.push_back(mfs[i]);
  }
  for (int j = 0; j < key_list.size(); j++) {
    keys.push_back(key_list[j]);
  }
}

//Constructor
DbList::DbList(neo4j_result_t *r, unsigned int ind, std::vector<std::string> key_list, ValueGenerationFunction mf, std::vector<KeyGenerationFunction> mfs) {
initialize(r, ind, key_list, mf, mfs);
}

DbList::DbList(neo4j_result_t *r, unsigned int ind, std::vector<std::string> key_list, std::vector<unsigned int> inds, ValueGenerationFunction mf, std::vector<KeyGenerationFunction> mfs, std::vector<IndexGenerationFunction> lfs) {
initialize(r, ind, key_list, mf, mfs);
for (int i = 0; i < lfs.size(); i++) {
  list_functions.push_back(lfs[i]);
}
for (int i = 0; i < inds.size(); i++) {
  indices.push_back(inds[i]);
}
}

//Get the value for this node/edge
neo4j_value_t DbList::get_list() {
if (!result) {
  return neo4j_null;
}
else {
  neo4j_value_t db_obj = neo4j_result_field(result, index);
  neo4j_value_t list = (*(list_function))(db_obj);
  for (int i = 0; i < map_functions.size(); i++) {
    neo4j_value_t key_str = neo4j_string(keys[i].c_str());
    list = (*(map_functions[i]))(list, key_str);
  }
  for (int j=0; j < list_functions.size(); j++) {
    list = (*(list_functions[j]))(list, indices[j]);
  }
  return list;
}
}

//Get a value from this list
neo4j_value_t DbList::get_list_value(unsigned int ind) {
  neo4j_value_t list = get_list();
  return neo4j_list_get(list, ind);
}

//Get the number of elements in the list
unsigned int DbList::size() {

//Retrieve the DB Object
neo4j_value_t db_obj = get_list();

//Get the map from the value using the map function
neo4j_value_t list = (*list_function)(db_obj);

//Return the map size
return neo4j_list_length(list);
}

//Get an element out of the list

bool DbList::get_bool_element(unsigned int ind) {
//Get the value from the map
neo4j_value_t element = get_list_value(ind);

//Convert the value to a float
return neo4j_bool_value(element);
}

int DbList::get_int_element(unsigned int ind) {
//Get the value from the map
neo4j_value_t element = get_list_value(ind);

//Convert the value to a float
return neo4j_int_value(element);
}

float DbList::get_float_element(unsigned int ind) {
//Get the value from the map
neo4j_value_t element = get_list_value(ind);

//Convert the value to a float
return neo4j_float_value(element);
}

std::string DbList::get_string_element(unsigned int ind, int char_buffer_size) {
//Get the value from the list
neo4j_value_t element = get_list_value(ind);

//Convert the value to a string
char db_cstr[char_buffer_size];
neo4j_string_value(element, db_cstr, char_buffer_size);
std::string db_str (db_cstr);
return db_str;
}

//Get a list element out of the list
DbListInterface* DbList::get_list_element(unsigned int ind) {
  indices.push_back(ind);
  list_functions.push_back(neo4j_list_get);
  return new DbList (result, index, keys, indices, list_function, map_functions, list_functions);
}

std::string DbList::to_string() {
neo4j_value_t value = get_list();
if (neo4j_eq(neo4j_null, value)) {
  return "";
}
char buf[128];
neo4j_tostring(value, buf, 128);
std::string ret_val (buf);
return ret_val;
}

//-----------------------------------------------------------------
//-----------------------DB Object---------------------------------
//-----------------------------------------------------------------

//-----------------------Internal----------------------------------

//Get a Value from a Result
neo4j_value_t DbObject::get_value() {
if (!result) {
  return neo4j_null;
}
else {
  return neo4j_result_field(result, index);
}
}

//Determine if a Value is of the specified type
bool DbObject::is_instance_of(neo4j_type_t type) {
neo4j_value_t value = get_value();
if (neo4j_eq(neo4j_null, value)) {
  return false;
}
if (neo4j_instanceof(value, type)) {
  return true;
}
return false;
}

//------------------------General----------------------------------

//Convert a Value to a string representation
std::string DbObject::to_string() {
neo4j_value_t value = get_value();
if (neo4j_eq(neo4j_null, value)) {
  return "";
}
char buf[128];
neo4j_tostring(value, buf, 128);
std::string ret_val (buf);
return ret_val;
}

//Get a map of the properties
DbMapInterface* DbObject::properties() {
if ( is_node() ) {
  return new DbMap (result, index, neo4j_node_properties);
}
else if ( is_edge() ){
  return new DbMap (result, index, neo4j_relationship_properties);
}
else {
  std::string err_msg = "Tried to retrieve properties for object that is neither a node or an edge";
  throw Neo4jException(err_msg);
}
}

//Get a list of node labels
DbListInterface* DbObject::labels() {
if ( is_node() ) {
  return new DbList (result, index, neo4j_node_labels);
}
else {
  std::string err_msg = "Tried to retrieve labels for object that is not a node";
  throw Neo4jException(err_msg);
}
}

//Get the relationship property
std::string DbObject::type() {
neo4j_value_t val = get_value();
if ( is_edge() ){
  char buf[256];
  neo4j_string_value(val, buf, 256);
  std::string ret_val (buf);
  return ret_val;
}
else {
  std::string err_msg = "Tried to retrieve type for object that is not an edge";
  throw Neo4jException(err_msg);
}
}

neo4j_value_t DbObject::get_identity() {
neo4j_value_t val = get_value();
if ( is_node() ) {
  return neo4j_node_identity(val);
}
else if ( is_edge() ){
  return neo4j_relationship_identity(val);
}
else {
  std::string err_msg = "Tried to retrieve properties for object that is neither a node or an edge";
  throw Neo4jException(err_msg);
}
}

//-----------------------------------------------------------------
//----------------------Neo4j Admin--------------------------------
//-----------------------------------------------------------------

//Start the admin
void Neo4jAdmin::initialize(const char * conn_str, bool secure) {

// Initialize the Neo4j Client
neo4j_client_init();

// Start up the connection
if (!secure) {
  connection = neo4j_connect(conn_str, NULL, NEO4J_INSECURE);
}

//Check if the connection was successful
if (!connection) {
  const char * err_string = strerror(errno);
  std::string err_msg (err_string);
  err_msg = "Error establishing connection: " + err_msg;
  throw Neo4jException(err_msg);
}

//Start a new session
session = neo4j_new_session(connection);

//Check if session creation was successful
if (!session) {
  const char * err_string = strerror(errno);
  std::string err_msg (err_string);
  err_msg = "Error starting session: " + err_msg;
  throw Neo4jException(err_msg);
}
}

//Execute a query and return the results in an iterator
ResultsIterator* Neo4jAdmin::execute(const char * query) {

//Execute the query
neo4j_result_stream_t *res = neo4j_run(session, query, neo4j_null);

//Check for a failure
int failure_check = neo4j_check_failure(res);
if (failure_check != 0) {
  const struct neo4j_failure_details *fd;
  fd = neo4j_failure_details(res);
  throw Neo4jException(fd->description);
}

//Return a results iterator for results access
return new ResultsIterator (res);
}
