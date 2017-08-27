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

#include <assert.h>
#include <string>
#include <unordered_map>
#include <iostream>
#include "include/neo4j_interface.h"
#include "include/factory_neo4j.h"

Neo4jInterface *neo = NULL;

int hello_world_test() {
  std::cout << "Running Hello World Test" << std::endl;
  // Execute a query
  ResultsIteratorInterface *results = NULL;
  std::string query = "RETURN 'hello world'";
  results = neo->execute(query);
  if (!results) return -1;

  // Access the results
  ResultTreeInterface* result = results->next();
  if (!result) return -1;
  DbObjectInterface* obj = result->get(0);
  std::string result_string = obj->to_string();
  std::cout << result_string << std::endl;
  assert(result_string == "hello world");

  delete obj;
  delete result;
  delete results;
  return 0;
}

int creation_test() {
  std::cout << "Running Object Creation Test" << std::endl;
  // Execute the query
  std::string query = \
    "CREATE (you:Person {name:'EA', list: [1, 2, 3]}) RETURN you";
  ResultsIteratorInterface *results = NULL;
  results = neo->execute(query);
  if (!results) return -1;

  // Access the results
  ResultTreeInterface* result = results->next();
  if (!result) return -1;
  DbObjectInterface* obj = result->get(0);
  std::string result_string = obj->to_string();
  std::cout << result_string << std::endl;

  assert(obj->is_node());

  DbMapInterface* map = obj->properties();
  std::cout << map->to_string() << std::endl;

  assert(map->element_exists("name"));
  assert(map->get_string_element("name") == "EA");
  assert(map->element_exists("list"));

  DbListInterface *list_prop = map->get_list_element("list");
  assert(list_prop->size() == 3);
  assert(list_prop->get_int_element(0) == 1);
  assert(list_prop->get_int_element(1) == 2);
  assert(list_prop->get_int_element(2) == 3);

  delete list_prop;
  delete map;
  delete obj;
  delete result;
  delete results;
  return 0;
}

int run_tests() {
  if (hello_world_test() != 0) return -1;
  if (creation_test() != 0) return -1;
  return 0;
}


// Print the element results from the tree
void print_results(ResultTreeInterface* result, int index) {
  DbObjectInterface* obj = result->get(index);
  std::string result_string = obj->to_string();
  std::cout << result_string << std::endl;
  delete obj;
}

// Check the types of the element in the tree
int check_types(ResultTreeInterface* result, int index) {
  DbObjectInterface* obj = result->get(index);
  int ret_val = -1;
  if ( obj->is_edge() ) {
    std::cout << "Element " << index << " found to be an edge." << std::endl;
    ret_val = 0;
  } else if ( obj->is_node() ) {
    std::cout << "Element " << index << " found to be a node." << std::endl;
    ret_val = 1;
  } else {
    std::cout << "Element " << index << " type not found." << std::endl;
  }
  delete obj;
  return ret_val;
}

void run_on_results(ResultsIteratorInterface *results) {
  int i = 0;
  if (results) {
    // Access the results
    ResultTreeInterface* result = results->next();

    if (result) {
      // Convert the results to strings
      print_results(result, 0);
      print_results(result, 1);
      print_results(result, 2);

      // Check the types of the elements in the result tree
      check_types(result, 0);
      check_types(result, 1);
      check_types(result, 2);

      // List & Map Tests

      // Properties
      std::cout << "Printing Property Maps" << std::endl;
      for (i = 0; i < 3; i++) {
        DbObjectInterface* obj = result->get(i);
        DbMapInterface* map = NULL;
        try {
          map = obj->properties();
        }
        catch (std::exception& e) {
          std::cout << "Exception Caught while printing property map" \
            << std::endl;
          std::cout << e.what() << std::endl;
        }
        if (map) {
          std::cout << map->to_string() << std::endl;
          delete map;
        }
        if (obj) {
          delete obj;
        }
      }

      std::cout << "Printing Name Property" << std::endl;
      for (i = 0; i < 3; i++) {
        DbObjectInterface* obj = result->get(i);
        DbMapInterface* map = NULL;
        if ( obj->is_edge() || obj->is_node() ) {
          try {
            map = obj->properties();
          }
          catch (std::exception& e) {
            std::cout << "Exception Caught while printing property map" \
              << std::endl;
            std::cout << e.what() << std::endl;
          }
          if (map) {
            if (map->element_exists("name")) {
              std::cout << map->get_string_element("name") << std::endl;
            }

            std::cout << "Printing List Property" << std::endl;
            if (map->element_exists("list")) {
              DbListInterface *list_prop = map->get_list_element("list");
              std::cout << "List Size: " << list_prop->size() << std::endl;
              std::cout << "List Contents: [" << list_prop->get_int_element(0) \
                << ", " << list_prop->get_int_element(1) << ", " << \
                list_prop->get_int_element(2) << "]" << std::endl;
              delete list_prop;
            }
            delete map;
          }
        }
        if (obj) {
          delete obj;
        }
      }

      // Labels
      std::cout << "Printing Label Lists" << std::endl;
      for (int i = 0; i < 3; i++) {
        DbObjectInterface* obj = result->get(i);
        DbListInterface* list = NULL;
        try {
          list = obj->labels();
        }
        catch (std::exception& e) {
          std::cout << "Exception Caught while showing label list" << std::endl;
          std::cout << e.what() << std::endl;
        }
        if (list) {
          std::cout << list->to_string() << std::endl;
          delete list;
        }
        if (obj) {
          delete obj;
        }
      }

      std::cout << "Printing Label" << std::endl;
      for (i = 0; i < 3; i++) {
        DbObjectInterface* obj = result->get(i);
        DbListInterface* list = NULL;
        if ( obj->is_node() ) {
          try {
            list = obj->labels();
            std::cout << list->get_string_element(0) << std::endl;
            delete list;
          }
          catch (std::exception& e) {
            std::cout << "Exception Caught while printing label list" \
              << std::endl;
            std::cout << e.what() << std::endl;
          }
        }
        if (obj) {
          delete obj;
        }
      }

      // Path Tests

      // Get the path
      DbObjectInterface* path = result->get(0);
      int path_size = -1;
      DbObjectInterface* path_obj = NULL;
      if (path->is_path()) {
        path_size = path->size();
        for (i = 0; i < path_size; i++) {
          std::cout << "Running Tests on path element " << i << std::endl;
          path_obj = path->get_path_element(i);
          std::cout << path_obj->to_string() << std::endl;

          // Run Properties test

          std::cout << "Printing Name Property" << std::endl;
          DbMapInterface* map = path_obj->properties();
          if (map->element_exists("name")) {
            std::cout << map->get_string_element("name") << std::endl;
          }

          // Run Node/Edge specific tests

          // We have a node
          if (path_obj->is_node()) {
            std::cout << "Node Detected" << std::endl;

            // Print the label list
            DbListInterface* list = path_obj->labels();
            int label_list_size = list->size();
            std::cout << "Label list: " << std::endl;
            for (int i = 0; i < label_list_size; i++) {
              std::cout << list->get_string_element(i) << std::endl;
            }
            delete list;
            list = NULL;
          } else {
            // We have an edge
            std::cout << "Edge Detected, type: " << path_obj->type() \
              << std::endl;
            std::cout << "Forward: ";
            if ( path_obj->forward() ) {
              std::cout << "true";
            } else {
              std::cout << "false";
            }
            std::cout << std::endl;
          }

          if (path_obj) {
            delete path_obj;
            path_obj = NULL;
          }
          if (map) {
            delete map;
            map = NULL;
          }
        }
      } else {
        std::cout << "Skipping path tests as the result object is not a path" \
          << std::endl;
      }
      delete path;

      // Cleanup
      delete result;
    }
  }
}

// Run a single test
void run_test(std::string query, std::string test_name) {
  std::cout << "Running Test: " << test_name << std::endl;

  // Execute a query
  ResultsIteratorInterface *results = NULL;
  try {
    results = neo->execute(query);
  }
  catch(std::exception& e) {
    std::cout << "Error running Query: " << e.what() << std::endl;
  }

  run_on_results(results);
  if (results) {delete results;}
}

// Run a single test
void run_test(std::string query, std::string test_name, \
  std::unordered_map<std::string, Neo4jQueryParameterInterface*> query_params) {
  std::cout << "Running Test: " << test_name << std::endl;
  // Execute a query
  ResultsIteratorInterface *results = NULL;

  try {
    results = neo->execute(query, query_params);
  }
  catch(std::exception& e) {
    std::cout << "Error running Query: " << e.what() << std::endl;
  }

  run_on_results(results);
  if (results) {delete results;}
}

// Main Method
int main(int argc, char** argv) {
  // Get the component factory
  Neo4jComponentFactory neo4j_factory;

  // Start the Neo4j Administrator
  if (argc > 1) {
    std::string conn_str(argv[1]);
    std::cout << conn_str << std::endl;
    neo = neo4j_factory.get_neo4j_interface(conn_str);
  } else {
    neo = neo4j_factory.get_neo4j_interface("neo4j://localhost:7687");
  }

  return run_tests();

  // Hello World
  run_test("RETURN 'hello world'", "Hello World");

  // Creation with Return
  run_test("CREATE (you:Person {name:'EA', list: [1, 2, 3]}) RETURN you", \
    "Creation with return");

  // Match
  run_test("MATCH (you:Person) WHERE you.name = 'EA' RETURN you", "Match");

  // Match & Create
  run_test("MATCH (you:Person {name:'EA'}) CREATE (you)-[like:LIKE]->(neo:Database {name:'Neo4j', list: [4, 5, 6]}) RETURN you, like, neo", \
    "Match & Create");

  // Query Failure
  run_test("MATCH (you:Person {name:'EA'}) CREATE (you)-[like:LIKE]->(neo:Database {name:'Neo4j', list: [1, 2, 3]) RETURN you, like, neo", \
    "Bad Query");

  // Query Parameters
  run_test("CREATE (you:Gelatin {name:1, list: [1, 2, 3]}) RETURN you", \
    "Query Parameters - Additional Data Addition");
  std::unordered_map<std::string, Neo4jQueryParameterInterface*> query_params1;
  Neo4jQueryParameterInterface* name_param = \
    neo4j_factory.get_neo4j_query_parameter(1);
  query_params1.emplace("inp_name", name_param);
  run_test("MATCH (you:Gelatin) WHERE you.name = {inp_name} RETURN you", \
    "Integer Query Parameters", query_params1);

  std::unordered_map<std::string, Neo4jQueryParameterInterface*> query_params2;
  Neo4jQueryParameterInterface* name_param2 = \
    neo4j_factory.get_neo4j_query_parameter("EA");
  query_params2.emplace("inp_name", name_param2);
  run_test("MATCH (you:Person) WHERE you.name = {inp_name} RETURN you", \
    "String Query Parameters", query_params2);

  // Path Test
  run_test("CREATE (base:CoordinateSystem {name: '1', list: [7, 8, 9]}) RETURN base", \
    "Path Test - Create Base");
  run_test("MATCH (base:CoordinateSystem {name: '1'}) CREATE (base)-[transform:Transform {matrix: [1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1]}]->(next:CoordinateSystem {name: '2', list: [10, 11, 12]}) RETURN base, transform, next", \
    "Path Test - Create First Connection");
  run_test("MATCH (base:CoordinateSystem {name: '1'})-[transform:Transform]->(next:CoordinateSystem {name: '2'}) CREATE (next)-[nexttransform:Transform {matrix: [1,1,0,1,0,1,0,1,1,0,1,1,0,0,0,1]}]->(final:CoordinateSystem {name: '3', list: [1, 2, 3]}) RETURN base, transform, next, nexttransform, final", "Path Test - Create Second Connection");
  run_test("MATCH (base:CoordinateSystem {name:'1'}), (next:CoordinateSystem {name:'3'}), p = shortestPath((base)-[r*]-(next)) RETURN p", \
    "Path Test");

  delete name_param;
  delete name_param2;
  delete neo;
}
