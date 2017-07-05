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

#ifndef AOSSL_FACTORY_NEO4J
#define AOSSL_FACTORY_NEO4J

#include "neo4j_interface.h"
#include "neo4j_admin.h"

//! The Neo4j Service Component Factory

//! The Service Component Factory tracks the Neo4j
//! objects exposed by the framework and passes back
//! instances of interfaces.  This allows for the publicly exposed methods
//! to be independent of the implementations.
class Neo4jComponentFactory {
 public:

  //! Create a new Service Component Factory
  Neo4jComponentFactory() {}

  //! Delete a Service Component Factory
  ~Neo4jComponentFactory() {}

  //! Get a Neo4j Interface instance
  Neo4jInterface* get_neo4j_interface(const char * conn_string) {return new Neo4jAdmin(conn_string);}
  //! Get a Neo4j Interface instance
  Neo4jInterface* get_neo4j_interface(std::string conn_string) {return new Neo4jAdmin(conn_string);}
  //! Get a Neo4j Interface instance
  Neo4jInterface* get_neo4j_interface(const char * conn_str, bool secure) {return new Neo4jAdmin(conn_str, secure);}
  //! Get a Neo4j Interface instance
  Neo4jInterface* get_neo4j_interface(std::string conn_str, bool secure) {return new Neo4jAdmin(conn_str, secure);}
  //! Get a Neo4j Interface instance
  Neo4jInterface* get_neo4j_interface(const char * conn_str, bool secure, int pool_size) {return new Neo4jAdmin(conn_str, secure, pool_size);}
  //! Get a Neo4j Interface instance
  Neo4jInterface* get_neo4j_interface(std::string conn_str, bool secure, int pool_size) {return new Neo4jAdmin(conn_str, secure, pool_size);}

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
