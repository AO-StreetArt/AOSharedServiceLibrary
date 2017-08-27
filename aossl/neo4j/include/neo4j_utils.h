#include <neo4j-client.h>

#ifndef AOSSL_NEO4J_INCLUDE_NEO4J_UTILS_H_
#define AOSSL_NEO4J_INCLUDE_NEO4J_UTILS_H_

// Neo4j Query Parameter Types
const int _BOOL_TYPE = 0;
const int _STR_TYPE = 1;
const int _INT_TYPE = 2;
const int _FLT_TYPE = 3;

// Typedefs of Neo4j Functions
typedef neo4j_value_t(*ValueGenerationFunction)(neo4j_value_t);
typedef neo4j_value_t(*KeyGenerationFunction)(neo4j_value_t, neo4j_value_t);
typedef neo4j_value_t(*IndexGenerationFunction)(neo4j_value_t, unsigned int);

#endif  // AOSSL_NEO4J_INCLUDE_NEO4J_UTILS_H_
