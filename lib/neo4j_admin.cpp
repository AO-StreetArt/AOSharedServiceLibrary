#include "include/neo4j_admin.h"

//-----------------------------------------------------------------
//--------------------Results Iterator-----------------------------
//-----------------------------------------------------------------

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
  for (std::size_t i = 0; i < mfs.size(); i++) {
    map_functions.push_back(mfs[i]);
  }
  for (std::size_t j = 0; j < key_list.size(); j++) {
    keys.push_back(key_list[j]);
  }
}

DbMap::DbMap(neo4j_result_t *r, int ind, std::vector<std::string> key_list, ValueGenerationFunction mf, std::vector<KeyGenerationFunction> mfs, int pindex) {
  result=r;
  index=ind;
  map_function = mf;
  for (std::size_t i = 0; i < mfs.size(); i++) {
    map_functions.push_back(mfs[i]);
  }
  for (std::size_t j = 0; j < key_list.size(); j++) {
    keys.push_back(key_list[j]);
  }
  path_index = pindex;
}

//Get the value for this node/edge
neo4j_value_t DbMap::get_map() {
if (!result) {
  return neo4j_null;
}
else {
  //Get the query result field
  neo4j_value_t db_obj = neo4j_result_field(result, index);
  //If the DB Value is a path, then we need to update it to the
  //respective element based on the path index
  if (path_index != -1) {
    //Check if we have a node or relationship
    if (path_index == 0) {
      //We have a node
      db_obj = neo4j_path_get_node(db_obj, path_index);
    }
    else if (path_index == 1) {
      //We have a relationship
      db_obj = neo4j_path_get_relationship(db_obj, 0, NULL);
    }
    else if (path_index % 2 == 0) {
      //We have a node
      db_obj = neo4j_path_get_node(db_obj, path_index/2);
    }
    else {
      //We have a relationship
      db_obj = neo4j_path_get_relationship(db_obj, (path_index-1)/2, NULL);
    }
  }
  //Get to the Neo4j Map via the map functions
  neo4j_value_t map = (*(map_function))(db_obj);
  for (std::size_t i = 0; i < map_functions.size(); i++) {
    neo4j_value_t key_str = neo4j_string(keys[i].c_str());
    map = (*(map_functions[i]))(map, key_str);
  }
  return map;
}
}

bool DbMap::element_exists(std::string key) {
//Get the value from the map
neo4j_value_t element = get_map_value(key);
if (neo4j_eq(neo4j_null, element)) {
  return false;
}
return true;
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

double DbMap::get_float_element(std::string key) {

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

DbListInterface* DbMap::get_list_element(std::string key) {
  return new DbList (result, index, map_function, path_index, neo4j_map_kget, key);
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

//Get the value for this node/edge
neo4j_value_t DbList::get_list() {
if (!result) {
  return neo4j_null;
}
else {
  //Get the DB Value
  neo4j_value_t db_obj = neo4j_result_field(result, index);
  //If the DB Value is a path, then we need to update it to the
  //respective element based on the path index
  if (path_index != -1) {
    //Check if we have a node or relationship
    if (path_index == 0) {
      //We have a node
      db_obj = neo4j_path_get_node(db_obj, path_index);
    }
    else if (path_index == 1) {
      //We have a relationship
      db_obj = neo4j_path_get_relationship(db_obj, 0, NULL);
    }
    else if (path_index % 2 == 0) {
      //We have a node
      db_obj = neo4j_path_get_node(db_obj, path_index/2);
    }
    else {
      //We have a relationship
      db_obj = neo4j_path_get_relationship(db_obj, (path_index-1)/2, NULL);
    }
  }
  neo4j_value_t list = (*(list_function))(db_obj);
  if (map_function) {
    return (*(map_function))(list, neo4j_string(map_key.c_str()));
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

//Return the map size
return neo4j_list_length(db_obj);
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

double DbList::get_float_element(unsigned int ind) {
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
  neo4j_value_t val = neo4j_result_field(result, index);
  //Check if this is the result of getting a value from a path
  if (path_index != -1) {
    //Check if we have a node or relationship
    if (path_index == 0) {
      //We have a node
      return neo4j_path_get_node(val, path_index);
    }
    else if (path_index == 1) {
      //We have a relationship
      return neo4j_path_get_relationship(val, 0, NULL);
    }
    else if (path_index % 2 == 0) {
      //We have a node
      return neo4j_path_get_node(val, path_index / 2);
    }
    else {
      //We have a relationship
      return neo4j_path_get_relationship(val, (path_index-1)/2, NULL);
    }
  }
  return val;
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
  return new DbMap (result, index, neo4j_node_properties, path_index);
}
else if ( is_edge() ){
  return new DbMap (result, index, neo4j_relationship_properties, path_index);
}
else {
  std::string err_msg = "Tried to retrieve properties for object that is neither a node or an edge";
  throw Neo4jException(err_msg);
}
}

//Get a list of node labels
DbListInterface* DbObject::labels() {
if ( is_node() ) {
  return new DbList (result, index, neo4j_node_labels, path_index);
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
  neo4j_value_t rel_type = neo4j_relationship_type(val);
  char buf[256];
  neo4j_string_value(rel_type, buf, 256);
  std::string ret_val (buf);
  return ret_val;
}
else {
  std::string err_msg = "Tried to retrieve type for object that is not an edge";
  throw Neo4jException(err_msg);
}
}

bool DbObject::forward() {
neo4j_value_t val = get_value();
if ( is_edge() && path_index != -1 ){
  bool is_forward;
  neo4j_path_get_relationship(val, path_index, &is_forward);
  return is_forward;
}
else {
  std::string err_msg = "Tried to retrieve relationship direction for object that is not an edge taken from a path";
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

unsigned int DbObject::size() {
neo4j_value_t val = get_value();
if ( is_path() ) {
  return (neo4j_path_length(val) * 2) + 1;
}
else {
  std::string err_msg = "Tried to retrieve path length for object that is not a path";
  throw Neo4jException(err_msg);
}
}

//-----------------------------------------------------------------
//----------------Neo4j Query Parameters---------------------------
//-----------------------------------------------------------------

void Neo4jQueryParameter::add_value(bool new_val) {
  std::string err_msg;
  if (!is_list) {
    err_msg = "Trying to add value to non-arry parameter";
    throw Neo4jException(err_msg);
  }
  else {
    if (type==-1 || type==_BOOL_TYPE) {
      bool_values.push_back(new_val);
      type=_BOOL_TYPE;
    }
    else {
      err_msg = "Only single-type arrays are supported";
      throw Neo4jException(err_msg);
    }
  }
}

void Neo4jQueryParameter::add_value(std::string new_val) {
  std::string err_msg;
  if (!is_list) {
    err_msg = "Trying to add value to non-arry parameter";
    throw Neo4jException(err_msg);
  }
  else {
    if (type==-1 || type==_STR_TYPE) {
      str_values.push_back(new_val);
      type=_STR_TYPE;
    }
    else {
      err_msg = "Only single-type arrays are supported";
      throw Neo4jException(err_msg);
    }
  }
}

void Neo4jQueryParameter::add_value(const char * new_val) {
  std::string new_str (new_val);
  add_value(new_str);
}

void Neo4jQueryParameter::add_value(int new_val) {
  std::string err_msg;
  if (!is_list) {
    err_msg = "Trying to add value to non-arry parameter";
    throw Neo4jException(err_msg);
  }
  else {
    if (type==-1 || type==_INT_TYPE) {
      int_values.push_back(new_val);
      type=_INT_TYPE;
    }
    else {
      err_msg = "Only single-type arrays are supported";
      throw Neo4jException(err_msg);
    }
  }
}

void Neo4jQueryParameter::add_value(float new_val) {
  if (!is_list) {
    std::string err_msg = "Trying to add value to non-arry parameter";
    throw Neo4jException(err_msg);
  }
  else {
    if (type==-1 || type==_FLT_TYPE) {
      double_values.push_back(new_val);
      type=_FLT_TYPE;
    }
    else {
      std::string err_msg = "Only single-type arrays are supported";
      throw Neo4jException(err_msg);
    }
  }
}

unsigned int Neo4jQueryParameter::size() {
  if (!is_list) {
    std::string err_msg = "Trying to take the size of a non-arry parameter";
    throw Neo4jException(err_msg);
  }
  else {
    if (type==_FLT_TYPE) {
      return double_values.size();
    }
    else if (type==_INT_TYPE) {
      return int_values.size();
    }
    else if (type==_STR_TYPE) {
      return str_values.size();
    }
    else if (type==_BOOL_TYPE) {
      return bool_values.size();
    }
    else {
      return 0;
    }
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

//Insert a map of parameters into a query,
//Execute the query and return the results in an iterator
ResultsIteratorInterface* Neo4jAdmin::execute(const char * query, std::unordered_map<std::string, Neo4jQueryParameterInterface*> query_params) {

//Get lists of keys and values for query parameters
std::vector<std::string> keys;
keys.reserve(query_params.size());


std::vector<Neo4jQueryParameterInterface*> vals;
vals.reserve(query_params.size());

for(auto kv : query_params) {
    keys.push_back(kv.first);
    vals.push_back(kv.second);
}

//Create an array of neo4j map entries
neo4j_map_entry_t map_entries[keys.size()];

for (unsigned int i = 0; i < keys.size(); i++) {
  Neo4jQueryParameterInterface* val = vals[i];
  int val_type = val->get_type();
  bool is_list = val->is_array();
  if (is_list) {
    //We have an array parameter
    unsigned int list_size = val->size();
    neo4j_value_t list_values[list_size];
    //Boolean Array
    if (val_type == _BOOL_TYPE) {
      for (unsigned int j=0;j<list_size;j++) {
        list_values[j] = neo4j_bool(val->get_boolean_value(j));
      }
    }
    //String Array
    else if (val_type == _STR_TYPE) {
      for (unsigned int j=0;j<list_size;j++) {
        list_values[j] = neo4j_string(val->get_string_value(j).c_str());
      }
    }
    //Integer Array
    else if (val_type == _INT_TYPE) {
      for (unsigned int j=0;j<list_size;j++) {
        list_values[j] = neo4j_int(val->get_integer_value(j));
      }
    }
    //Float Array
    else if (val_type == _FLT_TYPE) {
      for (unsigned int j=0;j<list_size;j++) {
        list_values[j] = neo4j_float(val->get_double_value(j));
      }
    }
    neo4j_value_t array_val = neo4j_list(list_values, list_size);
    map_entries[i] = neo4j_map_entry(keys[i].c_str(), array_val);
  }
  //We have single value parameter, and just need to determine the type
  else if (val_type == _BOOL_TYPE) {
    neo4j_value_t bool_val = neo4j_bool(val->get_boolean_value());
    map_entries[i] = neo4j_map_entry(keys[i].c_str(), bool_val);
  }
  else if (val_type == _STR_TYPE) {
    const char * cstr_value = val->get_string_value().c_str();
    const char * cstr_keyval = keys[i].c_str();
    neo4j_value_t str_val = neo4j_string(cstr_value);
    map_entries[i] = neo4j_map_entry(cstr_keyval, str_val);
  }
  else if (val_type == _INT_TYPE) {
    neo4j_value_t int_val = neo4j_int(val->get_integer_value());
    map_entries[i] = neo4j_map_entry(keys[i].c_str(), int_val);
  }
  else if (val_type == _FLT_TYPE) {
    neo4j_value_t float_val = neo4j_float(val->get_double_value());
    map_entries[i] = neo4j_map_entry(keys[i].c_str(), float_val);
  }
}

//Create a neo4j value of the map
neo4j_value_t param_map = neo4j_map (map_entries, keys.size());

//Execute the query
neo4j_result_stream_t *res = neo4j_run(session, query, param_map);

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
