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
        std::cout << map->get_string_element("name") << std::endl;
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

  //Cleanup
  delete result;
  delete results;
}
}

//Main Method
int main() {

//Start the Neo4j Administrator
Neo4jComponentFactory neo4j_factory;
neo = neo4j_factory.get_neo4j_interface("neo4j://neo4j:manage@localhost:7687");

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

delete neo;

}
