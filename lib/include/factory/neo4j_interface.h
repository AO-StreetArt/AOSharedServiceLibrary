#include <string>
#include <exception>

#ifndef AOSSL_NEO4J_INTERFACE
#define AOSSL_NEO4J_INTERFACE

//-----------------------------------------------------------------
//--------------------Neo4jException-------------------------------
//-----------------------------------------------------------------

//! A Neo4j Exception

//! A child class of std::exception
//! which holds error information
struct Neo4jException: public std::exception
{
  //! An error message passed on initialization
  std::string int_msg;

  //! Create a Neo4j Exception, and store the given error message
  Neo4jException (std::string msg) {int_msg = msg;}

  Neo4jException () {}
  ~Neo4jException() throw () {}
  //! Show the error message in readable format
  const char * what() const throw ()
  {
  std::string what_str = "Error in Neo4j Request: " + int_msg;
  return what_str.c_str();
  }
};

//-------------------------------------------------------------------
////---------------------Query Results-------------------------------
////-----------------------------------------------------------------

//! A Neo4j List

//! This is returned for list elements
//! in a map as well as node labels
class DbListInterface {
public:

  virtual ~DbListInterface() {}

  //! Get a list element out of a list
  virtual DbListInterface* get_list_element(unsigned int ind) = 0;

  //! Get a bool element out of a list
  virtual bool get_bool_element(unsigned int ind) = 0;

  //! Get an int element out of a list
  virtual int get_int_element(unsigned int ind) = 0;

  //! Get a float element out of a list
  virtual float get_float_element(unsigned int ind) = 0;

  //! Get a list string out of a list
  virtual std::string get_string_element(unsigned int ind, int char_buffer_size) = 0;

  //! Get a string element out of a list
  virtual std::string get_string_element(unsigned int ind) = 0;

  //! Get the string representation of the list
  virtual std::string to_string() = 0;

  //! Get the size of a list
  virtual unsigned int size() = 0;
};

//! A Neo4j Map

//! This is returned for map elements
//! in a map as well as node/edge properties
class DbMapInterface {
public:

  virtual ~DbMapInterface() {}

  //! Get the size of the map
  virtual unsigned int size() = 0;

  //! Get a string element out of a map
  virtual std::string get_string_element(std::string key, int char_buffer_size) = 0;

  //! Get a string element out of a map
  virtual std::string get_string_element(std::string key) = 0;

  //! Get a bool element out of a map
  virtual bool get_bool_element(std::string key) = 0;

  //! Get an int element out of a map
  virtual int get_int_element(std::string key) = 0;

  //! Get a float element out of a map
  virtual float get_float_element(std::string key) = 0;

  //! Get a map element out of a map
  virtual DbMapInterface* get_map_element(std::string key) = 0;

  //! Get a list element out of a map
  virtual DbListInterface* get_list_element(std::string key) = 0;

  //! Get the string representation of the map
  virtual std::string to_string() = 0;
};

//! A Neo4j Object

//! This is returned from a result tree
//! and represents either a node or an edge
class DbObjectInterface {
public:

  virtual ~DbObjectInterface() {}

  //! Is this a node?

  //! Return true if this is a node object.
  //! Return false if this is an edge object.
  virtual bool is_node() = 0;

  //! Is this an edge?

  //! Return false if this is a node object.
  //! Return true if this is an edge object.
  virtual bool is_edge() = 0;

  //! Get the string representation of the object
  virtual std::string to_string() = 0;

  //! Get the properties of the object

  //! This functions for both node objects and
  //! edge objects
  virtual DbMapInterface* properties() = 0;

  //! Get the labels of the node

  //! This functions only for node
  //! objects
  virtual DbListInterface* labels() = 0;

  //! Get the type of the edge

  //! This functions only for edge
  //! objects
  virtual std::string type() = 0;
};

//! Tree of Query Results

// Represents a single query Result, returned by the iterator
// Consists of a set of nodes and edges
class ResultTreeInterface {
public:

  virtual ~ResultTreeInterface() {}

  //! Get the node/edge at the specified index
  virtual DbObjectInterface* get(int index) = 0;
};

//! Results Iterator for viewing query results

//! Accepts a result stream as an input, returns Results
//! Trees for each entry that matches in the query
//! Deletes it when finished
class ResultsIteratorInterface {
public:

  virtual ~ResultsIteratorInterface() {}

  //! Clear the results iterator of all values
  virtual void clear() = 0;

  //! Is the iterator empty?

  //! Return true if this iterator contains no results.
  //! Return false otherwise.
  virtual bool empty() = 0;

  //! Get the number of results in the query
  virtual unsigned int length() = 0;

  //! Get the next query result
  virtual ResultTreeInterface* next() = 0;
};

//-------------------------------------------------------------------
////----------------------Neo4j Admin--------------------------------
////-----------------------------------------------------------------

//! Neo4j Query Interface

//! Executes queries against the Neo4j DB.
//! Returns complex data structures for viewing
//! Query results, which start with the ResultsIteratorInterface
class Neo4jInterface {
public:

  virtual ~Neo4jInterface() {};

  //! Execute the given Cypher Query
  virtual ResultsIteratorInterface* execute(const char * query) = 0;

  //! Execute the given Cypher Query
  virtual ResultsIteratorInterface* execute(std::string query) = 0;
};

#endif
