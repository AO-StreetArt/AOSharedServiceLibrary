#include "factory/neo4j_interface.h"
#include "neo4j_admin.h"

#ifndef AOSSL_FACTORY_NEO4J
#define AOSSL_FACTORY_NEO4J

//! The Neo4j Service Component Factory

//! The Service Component Factory tracks the Neo4j
//! objects exposed by the framework and passes back
//! instances of interfaces.  This allows for the publicly exposed methods
//! to be independent of the implementations.
class Neo4jComponentFactory
{
public:

  //! Create a new Service Component Factory
  Neo4jComponentFactory() {}

  //! Delete a Service Component Factory
  ~Neo4jComponentFactory() {}

  //! Get a Neo4j Interface instance
  Neo4jInterface* get_neo4j_interface(const char * conn_string) {return new Neo4jAdmin(conn_string);}
  //! Get a Neo4j Interface instance
  Neo4jInterface* get_neo4j_interface(std::string conn_string) {return new Neo4jAdmin(conn_string);}

  //! Get a Neo4j Array Query Parameter
  Neo4jQueryParameterInterface* get_neo4j_query_parameter() {return new Neo4jQueryParameter();}
  //! Get a Neo4j Query Parameter
  Neo4jQueryParameterInterface* get_neo4j_query_parameter(bool inp_bool) {return new Neo4jQueryParameter(inp_bool);}
  //! Get a Neo4j Query Parameter
  Neo4jQueryParameterInterface* get_neo4j_query_parameter(std::string inp_str) {return new Neo4jQueryParameter(inp_str);}
  //! Get a Neo4j Query Parameter
  Neo4jQueryParameterInterface* get_neo4j_query_parameter(const char * inp_str) {return new Neo4jQueryParameter(inp_str);}
  //! Get a Neo4j Query Parameter
  Neo4jQueryParameterInterface* get_neo4j_query_parameter(int inp_int) {return new Neo4jQueryParameter(inp_int);}
  //! Get a Neo4j Query Parameter
  Neo4jQueryParameterInterface* get_neo4j_query_parameter(double inp_double) {return new Neo4jQueryParameter(inp_double);}

};

#endif
