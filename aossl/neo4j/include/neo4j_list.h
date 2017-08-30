// A List of values returned from Neo4j
#include "neo4j_interface.h"
#include "neo4j_utils.h"
#include <neo4j-client.h>
#include <string>

#ifndef AOSSL_NEO4J_INCLUDE_NEO4J_LIST_H_
#define AOSSL_NEO4J_INCLUDE_NEO4J_LIST_H_

class DbList: public DbListInterface {
  // Internal Variables
  neo4j_result_t *result = NULL;
  int index;
  int path_index;
  ValueGenerationFunction list_function;
  KeyGenerationFunction map_function = NULL;
  std::string map_key;
  char db_cstr[512] = "";
  char buf[512] = "";
  std::string elt = "";
  std::string str_val = "";

  // Retrieve the Neo4j List for this node/edge
  inline neo4j_value_t get_list() {
    if (!result) {
      return neo4j_null;
    } else {
      // Get the DB Value
      neo4j_value_t db_obj = neo4j_result_field(result, index);
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
      neo4j_value_t list = (*(list_function))(db_obj);
      if (map_function) {
        return (*(map_function))(list, neo4j_string(map_key.c_str()));
      }
      return list;
    }
  }

  // Get a value out of the list
  inline neo4j_value_t get_list_value(unsigned int ind) {
    neo4j_value_t list = get_list();
    return neo4j_list_get(list, ind);
  }

  // Base Initializer
  inline void init(neo4j_result_t *r, unsigned int ind, \
    ValueGenerationFunction lf) {
    result = r;
    index = ind;
    list_function = lf;
  }

 public:
  // Constructors
  inline DbList(neo4j_result_t *r, unsigned int ind, \
    ValueGenerationFunction lf) {
    init(r, ind, lf);
  }

  inline DbList(neo4j_result_t *r, unsigned int ind, \
    ValueGenerationFunction lf, unsigned int pindex) {
    init(r, ind, lf);
    path_index = pindex;
  }

  inline DbList(neo4j_result_t *r, unsigned int ind, \
    ValueGenerationFunction lf, unsigned int pindex, \
    KeyGenerationFunction mf, std::string mf_key) {
    init(r, ind, lf);
    path_index = pindex;
    map_function = mf;
    map_key = mf_key;
  }

  // Destructor
  ~DbList() {}

  // Element Retrieval
  // Retrieve a boolean element out of the list
  inline bool get_bool_element(unsigned int ind) {
    // Get the value from the map
    neo4j_value_t element = get_list_value(ind);

    // Convert the value to a float
    return neo4j_bool_value(element);
  }
  // Retrieve an integer element out of the list
  inline int get_int_element(unsigned int ind) {
    // Get the value from the map
    neo4j_value_t element = get_list_value(ind);

    // Convert the value to a float
    return neo4j_int_value(element);
  }
  // Retrieve a float element out of the list
  inline double get_float_element(unsigned int ind) {
    // Get the value from the map
    neo4j_value_t element = get_list_value(ind);

    // Convert the value to a float
    return neo4j_float_value(element);
  }
  // Retrieve a string element out of the list
  inline std::string get_string_element(unsigned int ind, int char_buffer_size) {
    // Get the value from the list
    neo4j_value_t element = get_list_value(ind);

    // Convert the value to a string
    neo4j_string_value(element, db_cstr, char_buffer_size);
    elt.assign(db_cstr);
    return elt;
  }
  // Retrieve a string element out of the list
  inline std::string get_string_element(unsigned int ind) {
    return get_string_element(ind, 512);
  }

  // Convert the List to a string representation
  inline std::string to_string() {
    neo4j_value_t value = get_list();
    if (neo4j_eq(neo4j_null, value)) {
      return "";
    }
    neo4j_tostring(value, buf, 512);
    str_val.assign(buf);
    return str_val;
  }

  // How many elements are in the list
  inline unsigned int size() {
    // Retrieve the DB Object
    neo4j_value_t db_obj = get_list();

    // Return the map size
    return neo4j_list_length(db_obj);
  }
};

#endif  // AOSSL_NEO4J_INCLUDE_NEO4J_LIST_H_
