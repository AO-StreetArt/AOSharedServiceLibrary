#include "neo4j_interface.h"
#include "neo4j_map.h"
#include "neo4j_list.h"
#include <neo4j-client.h>
#include <string>

#ifndef AOSSL_NEO4J_INCLUDE_NEO4J_OBJECT_H_
#define AOSSL_NEO4J_INCLUDE_NEO4J_OBJECT_H_

// Represents a single node, edge, or path
class DbObject: public DbObjectInterface {
  // Internal Variables
  neo4j_result_t *result = NULL;
  int index;
  int path_index;
  bool is_forward = true;
  neo4j_value_t val;
  std::string str_val = "";
  char buf[128] = "";

  // Get the neo4j value for this object
  neo4j_value_t get_value() {
    if (!result) {
      return neo4j_null;
    } else {
      val = neo4j_result_field(result, index);
      // Check if this is the result of getting a value from a path
      if (path_index != -1) {
        // Check if we have a node or relationship
        if (path_index == 0) {
          // We have a node
          return neo4j_path_get_node(val, path_index);
        } else if (path_index == 1) {
          // We have a relationship
          return neo4j_path_get_relationship(val, 0, NULL);
        } else if (path_index % 2 == 0) {
          // We have a node
          return neo4j_path_get_node(val, path_index / 2);
        } else {
          // We have a relationship
          return neo4j_path_get_relationship(val, (path_index-1)/2, NULL);
        }
      }
      return val;
    }
  }

  // Is this object an instance of a given type?
  bool is_instance_of(neo4j_type_t type) {
    neo4j_value_t value = get_value();
    if (neo4j_eq(neo4j_null, value)) {
      return false;
    }
    if (neo4j_instanceof(value, type)) {
      return true;
    }
    return false;
  }

  // Base Initializer
  void init(neo4j_result_t *r, int ind) {
    result = r;
    index = ind;
  }

 public:
  // Constructors
  inline DbObject(neo4j_result_t *r, int ind) {
    init(r, ind);
    path_index = -1;
  }

  inline DbObject(neo4j_result_t *r, int ind, int ind2) {
    init(r, ind);
    path_index = ind2;
  }

  ~DbObject() {}

  // Type queries
  bool is_node() {return is_instance_of(NEO4J_NODE);}
  bool is_edge() {return is_instance_of(NEO4J_RELATIONSHIP);}
  bool is_path() {return is_instance_of(NEO4J_PATH);}

  // Convert the object to a string representation
  std::string to_string() {
    neo4j_value_t value = get_value();
    if (neo4j_eq(neo4j_null, value)) {
      return "";
    }
    neo4j_tostring(value, buf, 128);
    str_val.assign(buf);
    return str_val;
  }

  // Get the object properties
  DbMapInterface* properties() {
    if ( is_node() ) {
      return new DbMap(result, index, neo4j_node_properties, path_index);
    } else if ( is_edge() ) {
      return new DbMap(result, index, neo4j_relationship_properties, path_index);
    } else {
      std::string err_msg = \
        "Tried to retrieve properties for invalid object";
      throw Neo4jException(err_msg);
    }
  }

  // Get the object labels
  DbListInterface* labels() {
    if ( is_node() ) {
      return new DbList(result, index, neo4j_node_labels, path_index);
    } else {
      std::string err_msg = \
        "Tried to retrieve labels for object that is not a node";
      throw Neo4jException(err_msg);
    }
  }

  // Get the relationship type
  std::string type() {
    neo4j_value_t val = get_value();
    if ( is_edge() ) {
      neo4j_value_t rel_type = neo4j_relationship_type(val);
      char buf[256];
      neo4j_string_value(rel_type, buf, 256);
      std::string ret_val(buf);
      return ret_val;
    } else {
      std::string err_msg = \
        "Tried to retrieve type for object that is not an edge";
      throw Neo4jException(err_msg);
    }
  }

  // Is the edge going forward?
  bool forward() {
    neo4j_value_t val = get_value();
    if ( is_edge() && path_index != -1 ) {
      neo4j_path_get_relationship(val, path_index, &is_forward);
      return is_forward;
    } else {
      std::string err_msg = "Tried to pull relationship dir on invalid object";
      throw Neo4jException(err_msg);
    }
  }

  // Node Identity
  neo4j_value_t get_identity() {
    neo4j_value_t val = get_value();
    if ( is_node() ) {
      return neo4j_node_identity(val);
    } else if ( is_edge() ) {
      return neo4j_relationship_identity(val);
    } else {
      std::string err_msg = "Tried to call get identity on invalid object";
      throw Neo4jException(err_msg);
    }
  }

  inline bool is_equal_to(DbObject& comp_obj) {
    return neo4j_eq(get_identity(), comp_obj.get_identity());
  }

  unsigned int size() {
    neo4j_value_t val = get_value();
    if ( is_path() ) {
      return (neo4j_path_length(val) * 2) + 1;
    } else {
      std::string err_msg = "Tried to pull path length on invalid object";
      throw Neo4jException(err_msg);
    }
  }

  inline DbObjectInterface* get_path_element(int path_index) {
    return new DbObject(result, index, path_index);
  }
};

#endif  // AOSSL_NEO4J_INCLUDE_NEO4J_OBJECT_H_
