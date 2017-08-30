#include "neo4j_interface.h"
#include "neo4j_object.h"
#include <neo4j-client.h>
#include <string>

// Represents a single query Result, returned by the iterator
// Consists of a set of nodes and edges
class ResultTree: public ResultTreeInterface {
  neo4j_result_t *result = NULL;
 public:
  ResultTree(neo4j_result_t *r) {result = r;}
  ~ResultTree() {}
  bool exists() {if (result) {return true;} else {return false;}}
  DbObjectInterface* get(int index) {return new DbObject(result, index);}
};
