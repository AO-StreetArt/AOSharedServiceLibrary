# Neo4j Client

Neo4j is a graph based database which stores data in Nodes & Edges.  We start by retrieving our component factory & interface.

    #include "include/aossl/neo4j/neo4j_interface.h"
    #include "include/aossl/neo4j/factory_neo4j.h"

    Neo4jComponentFactory neo4j_factory;
    Neo4jInterface *neo = neo4j_factory.get_neo4j_interface("neo4j://localhost:7687");

Now, we can execute Cypher queries with the command below:

    std::string query = "CREATE (you:Person {name:'E', list: [1, 2, 3]})-[like:LIKE]->(me:Person {name:'F', list: [4, 5, 6]}) RETURN you, like, me";
    ResultsIteratorInterface *results = neo->execute(query);

Our results iterator lets us view the results of the query:

    ResultTreeInterface* result = results->next();
    DbObjectInterface* obj = result->get(0);
    std::cout << obj->to_string() << std::endl;

The result tree represents a single set of results.  This can include many nodes and/or edges, and depends entirely on the RETURN clause of the cypher query.  We can retrieve values by index from this tree into the DbObject, which is either a node, edge, or path.

We can access properties of nodes & edges:

    DbMapInterface* map = obj->properties();
    if (map->element_exists("name")) {
      std::cout << map->get_string_element("name") << std::endl;
    }
    if (map->element_exists("list")) {
      DbListInterface *list_prop = map->get_list_element("list");
      std::cout << "List Size: " << list_prop->size() << std::endl;
      std::cout << list_prop->get_int_element(0) << std::endl;
      delete list_prop;
    }

As well as labels of nodes and directions of edges:

    DbListInterface* list = obj->labels();
    std::cout << list->get_string_element(0) << std::endl;
    DbObjectInterface* edge = result->get(1);
    if (edge->forward()) {
      std::cout << "Forward edge" << std::endl;
    }

Remember to delete the objects that you've used when viewing the results!

    delete results;
    delete result;
    delete obj;
    delete map;
    delete list;
    delete edge;

## Query Parameters

We can pass parameters to queries by creating named maps with any number of parameters:

    std::unordered_map<std::string, Neo4jQueryParameterInterface*> query_params;
    Neo4jQueryParameterInterface* name_param = neo4j_factory.get_neo4j_query_parameter("E");
    query_params.emplace("inp_name", name_param);

Then, we can use the name we define as a variable, surrounded by "{}" within the query, and we pass the map to the execute function.

    std::string query = "MATCH (you:Person) WHERE you.name = {inp_name} RETURN you"
    ResultsIteratorInterface *results = neo->execute(query, query_params);

We can then access the results as above through the iterator.

## Paths

Certain queries return path objects instead of nodes and edges:

    std::string query = "MATCH (base:Person {name:'E'}), (next:Person {name:'F'}), p = shortestPath((base)-[r*]-(next)) RETURN p"
    ResultsIteratorInterface *results = neo->execute(query);

Then, we can access the results:

    ResultTreeInterface* result = results->next();
    DbObjectInterface* obj = result->get(0);
    DbObjectInterface* path_obj = NULL;
    if (obj->is_path()) {
      int path_size = path->size();
      DbObjectInterface* path_obj = path->get_path_element(i);
      std::cout << path_obj->to_string() << std::endl;
    }
