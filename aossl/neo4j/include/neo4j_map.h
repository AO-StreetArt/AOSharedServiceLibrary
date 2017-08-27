// A map of values returned from Neo4j
#include "neo4j_interface.h"
#include "neo4j_utils.h"
#include "neo4j_list.h"
#include <neo4j-client.h>
#include <string>

#ifndef AOSSL_NEO4J_INCLUDE_NEO4J_MAP_H_
#define AOSSL_NEO4J_INCLUDE_NEO4J_MAP_H_

class DbMap: public DbMapInterface {
  // Internal Variables
  neo4j_result_t *result = NULL;
  int index;
  int path_index;
  ValueGenerationFunction map_function;
  std::vector<KeyGenerationFunction> map_functions;
  std::vector<std::string> keys;
  char db_cstr[512] = "";
  char buf[128] = "";
  std::string elt = "";
  std::string str_val = "";
  neo4j_value_t db_obj;
  neo4j_value_t map;
  neo4j_value_t map_elt;
  std::mutex data_mutex;

  // Get the map for this node/edge
  void get_map() {
    db_obj = neo4j_null;
    map = neo4j_null;
    if (result) {
      // Get the query result field
      db_obj = neo4j_result_field(result, index);
      if ( neo4j_instanceof(db_obj, NEO4J_PATH) ) {
        // If the DB Value is a path, then we need to update it to the
        // respective element based on the path index
        if (path_index != -1) {
          // Check if we have a node or relationship
          if (path_index == 0) {
            // We have a node
            db_obj = neo4j_path_get_node(db_obj, path_index);
          } else if (path_index == 1) {
            // We have a relationship
            db_obj = neo4j_path_get_relationship(db_obj, 0, NULL);
          } else if (path_index % 2 == 0) {
            // We have a node
            db_obj = neo4j_path_get_node(db_obj, path_index/2);
          } else {
            // We have a relationship
            db_obj = neo4j_path_get_relationship(db_obj, (path_index-1)/2, NULL);
          }
        }
      }
      if ( neo4j_instanceof(db_obj, NEO4J_NODE) || neo4j_instanceof(db_obj, \
        NEO4J_RELATIONSHIP) ) {
        // Get to the Neo4j Map via the map functions
        map = (*(map_function))(db_obj);
        for (std::size_t i = 0; i < map_functions.size(); i++) {
          neo4j_value_t key_str = neo4j_string(keys[i].c_str());
          map = (*(map_functions[i]))(map, key_str);
        }
      }
    }
  }

  // Get a value from the map
  void get_map_value(std::string key) {
    get_map();
    map_elt = neo4j_map_get(map, key.c_str());
  }

  // Base Initializer
  void init(neo4j_result_t *r, int ind, ValueGenerationFunction mf) {
    result = r;
    index = ind;
    map_function = mf;
  }

 public:
  // Constructors
  inline DbMap(neo4j_result_t *r, int ind, ValueGenerationFunction mf) \
    {init(r, ind, mf);}

  inline DbMap(neo4j_result_t *r, int ind, ValueGenerationFunction mf, \
    int pindex) {
    init(r, ind, mf);
    path_index = pindex;
  }

  inline DbMap(neo4j_result_t *r, int ind, std::vector<std::string> key_list, \
    ValueGenerationFunction mf, std::vector<KeyGenerationFunction> mfs) {
    init(r, ind, mf);
    for (std::size_t i = 0; i < mfs.size(); i++) {
      map_functions.push_back(mfs[i]);
    }
    for (std::size_t j = 0; j < key_list.size(); j++) {
      keys.push_back(key_list[j]);
    }
  }

  inline DbMap(neo4j_result_t *r, int ind, std::vector<std::string> key_list, \
    ValueGenerationFunction mf, std::vector<KeyGenerationFunction> mfs, \
    int pindex) {
    init(r, ind, mf);
    for (std::size_t i = 0; i < mfs.size(); i++) {
      map_functions.push_back(mfs[i]);
    }
    for (std::size_t j = 0; j < key_list.size(); j++) {
      keys.push_back(key_list[j]);
    }
    path_index = pindex;
  }

  // Destructor
  ~DbMap() {}

  // How many elements are in the map
  inline unsigned int size() {
    std::lock_guard<std::mutex> lock(data_mutex);

    // Retrieve the map
    get_map();

    // Return the map size
    return neo4j_map_size(map);
  }

  // Does an element exist in the map
  inline bool element_exists(std::string key) {
    std::lock_guard<std::mutex> lock(data_mutex);
    // Get the value from the map
    get_map_value(key);
    if (neo4j_eq(neo4j_null, map_elt)) {
      return false;
    }
    return true;
  }

  // Element retrieval
  // Get a string element from the map
  inline std::string get_string_element(std::string key, int char_buffer_size) {
    std::lock_guard<std::mutex> lock(data_mutex);

    // Get the value from the map
    get_map_value(key);

    // Convert the value to a string
    neo4j_string_value(map_elt, db_cstr, char_buffer_size);
    if (db_cstr && db_cstr[0] != '\0') {
      elt.assign(db_cstr);
    } else {elt = "";}
    return elt;
  }

  // Get a string element from the map
  inline std::string get_string_element(std::string key) {
    return get_string_element(key, 512);
  }
  // Get a boolean element from the map
  inline bool get_bool_element(std::string key) {
    std::lock_guard<std::mutex> lock(data_mutex);

    // Get the value from the map
    get_map_value(key);

    // Convert the value to a boolean
    return neo4j_bool_value(map_elt);
  }
  // Get an integer element from the map
  inline int get_int_element(std::string key) {
    std::lock_guard<std::mutex> lock(data_mutex);

    // Get the value from the map
    get_map_value(key);

    // Convert the value to an int
    return neo4j_int_value(map_elt);
  }
  // Get a float element from the map
  inline double get_float_element(std::string key) {
    std::lock_guard<std::mutex> lock(data_mutex);

    // Get the value from the map
    get_map_value(key);

    // Convert the value to a float
    return neo4j_float_value(map_elt);
  }
  // Get a list element from the map
  inline DbListInterface* get_list_element(std::string key) {
    return new DbList(result, index, map_function, path_index, \
      neo4j_map_kget, key);
  }

  // Convert the map to a string representation
  inline std::string to_string() {
    std::lock_guard<std::mutex> lock(data_mutex);
    get_map();
    if (neo4j_eq(neo4j_null, map)) {
      return "";
    }
    neo4j_tostring(map, buf, 128);
    str_val.assign(buf);
    return str_val;
  }
};

#endif  // AOSSL_NEO4J_INCLUDE_NEO4J_MAP_H_
