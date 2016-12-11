#include "include/factory/neo4j_interface.h"
#include "include/factory_neo4j.h"
#include <iostream>
#include <string>

Neo4jInterface *neo = NULL;

//Print the element results from the tree
void print_results(ResultTreeInterface* result, int index) {
DbObjectInterface* obj = result->get(index);
std::string result_string = obj->to_string();
std::cout << result_string << std::endl;
delete obj;
}

//Check the types of the element in the tree
void check_types(ResultTreeInterface* result, int index) {
DbObjectInterface* obj = result->get(index);
if ( obj->is_edge() ) {
  std::cout << "Element " << index << " found to be an edge." << std::endl;
}
else if ( obj->is_node() ) {
  std::cout << "Element " << index << " found to be a node." << std::endl;
}
else {
  std::cout << "Element " << index << " type not found." << std::endl;
}
delete obj;
}

//Run a single test
void run_test(std::string query, std::string test_name) {
int i = 0;

std::cout << "Running Test: " << test_name << std::endl;

//Execute a query
ResultsIteratorInterface *results = NULL;
try {
  results = neo->execute(query);
}
catch (std::exception& e) {
  std::cout << "Error running Query: " << e.what() << std::endl;
}

if (results) {
  //Access the results
  ResultTreeInterface* result = results->next();

  //Convert the results to strings
  print_results(result, 0);
  print_results(result, 1);
  print_results(result, 2);

  //Check the types of the elements in the result tree
  check_types(result, 0);
  check_types(result, 1);
  check_types(result, 2);

  //List & Map Tests

  //Properties
  std::cout << "Printing Property Maps" << std::endl;
  for (i = 0; i < 3; i++) {
    DbObjectInterface* obj = result->get(i);
    DbMapInterface* map = NULL;
    try {
      map = obj->properties();
    }
    catch (std::exception& e) {
      std::cout << "Exception Caught while printing property map" << std::endl;
      std::cout << e.what() << std::endl;
    }
    if (map) {
      std::cout << map->to_string() << std::endl;
      delete map;
    }
    delete obj;
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
        std::cout << "Exception Caught while printing property map" << std::endl;
        std::cout << e.what() << std::endl;
      }
      if (map) {
        if (map->element_exists("name")) {
          std::cout << map->get_string_element("name") << std::endl;
        }
        delete map;
      }
      delete obj;
    }
  }

  //Labels
  std::cout << "Printing Label Lists" << std::endl;
  for (int i = 0; i < 3; i++) {
    DbObjectInterface* obj = result->get(i);
    DbListInterface* list = NULL;
    try {
      list = obj->labels();
    }
    catch (std::exception& e) {
      std::cout << "Exception Caught while printing label list" << std::endl;
      std::cout << e.what() << std::endl;
    }
    if (list) {
      std::cout << list->to_string() << std::endl;
      delete list;
    }
    delete obj;
  }

  std::cout << "Printing Label" << std::endl;
  for (i = 0; i < 3; i++) {
    DbObjectInterface* obj = result->get(i);
    DbListInterface* list = NULL;
    if ( obj->is_node() ) {
      try {
        list = obj->labels();
      }
      catch (std::exception& e) {
        std::cout << "Exception Caught while printing label list" << std::endl;
        std::cout << e.what() << std::endl;
      }
      if (list) {
        std::cout << list->get_string_element(0) << std::endl;
        delete list;
      }
      delete obj;
    }
  }

  //Path Tests

  //Get the path
  DbObjectInterface* path = result->get(0);
  int path_size = -1;
  DbObjectInterface* path_obj = NULL;
  if (path->is_path()) {
    path_size = path->size();
    for (i = 0; i < path_size; i++) {
      std::cout << "Running Tests on path element " << i << std::endl;
      path_obj = path->get_path_element(i);
      std::cout << path_obj->to_string() << std::endl;

      //Run Properties test

      std::cout << "Printing Name Property" << std::endl;
      DbMapInterface* map = path_obj->properties();
      if (map->element_exists("name")) {
        std::cout << map->get_string_element("name") << std::endl;
      }

      //Run Node/Edge specific tests

      //We have a node
      if (path_obj->is_node()) {
        std::cout << "Node Detected" << std::endl;

        //Print the label list
        DbListInterface* list = path_obj->labels();
        int label_list_size = list->size();
        std::cout << "Label list: " << std::endl;
        for (int i = 0; i < label_list_size; i++) {
          std::cout << list->get_string_element(i) << std::endl;
        }
      }
      //We have an edge
      else {
        std::cout << "Edge Detected, type: " << path_obj->type() << std::endl;
        std::cout << "Forward: ";
        if ( path_obj->forward() ) {
          std::cout << "true";
        }
        else {
          std::cout << "false";
        }
        std::cout << std::endl;
      }

      delete path_obj;
      path_obj = NULL;
    }
  }
  else {
    std::cout << "Skipping path tests as the result object is not a path" << std::endl;
  }
  delete path;

  //Cleanup
  delete result;
  delete results;
}
}

//Main Method
int main() {

//Start the Neo4j Administrator
Neo4jComponentFactory neo4j_factory;
neo = neo4j_factory.get_neo4j_interface("neo4j://localhost:7687");

//Hello World
run_test("RETURN 'hello world'", "Hello World");

//Creation with Return
run_test("CREATE (you:Person {name:'E'}) RETURN you", "Creation with return");

//Match
run_test("MATCH (you:Person {name:'E'}) RETURN you", "Match");

//Match & Create
run_test("MATCH (you:Person {name:'E'}) CREATE (you)-[like:LIKE]->(neo:Database {name:'Neo4j'}) RETURN you, like, neo", "Match & Create");

//Query Failure
run_test("MATCH (you:Person {name:'E'}) CREATE (you)-[like:LIKE]->(neo:Database {name:'Neo4j') RETURN you, like, neo", "Bad Query");

//Path Test
run_test("CREATE (base:CoordinateSystem {name: '1'}) RETURN base", "Path Test - Create Base");
run_test("MATCH (base:CoordinateSystem {name: '1'}) CREATE (base)-[transform:Transform {matrix: [1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1]}]->(next:CoordinateSystem {name: '2'}) RETURN base, transform, next", "Path Test - Create First Connection");
run_test("MATCH (base:CoordinateSystem {name: '1'})-[transform:Transform]->(next:CoordinateSystem {name: '2'}) CREATE (next)-[nexttransform:Transform {matrix: [1,1,0,1,0,1,0,1,1,0,1,1,0,0,0,1]}]->(final:CoordinateSystem {name: '3'}) RETURN base, transform, next, nexttransform, final", "Path Test - Create Second Connection");
run_test("MATCH (base:CoordinateSystem {name:'1'}), (next:CoordinateSystem {name:'3'}), p = shortestPath((base)-[r*]-(next)) RETURN p", "Path Test");

delete neo;

}
