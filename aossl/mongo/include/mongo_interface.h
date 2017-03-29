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

#ifndef AOSSL_MONGO_INTERFACE
#define AOSSL_MONGO_INTERFACE

#include <string>
#include <exception>
#include <vector>

//! Mongo Exception, used to store errors passed from Mongo
struct MongoException: public std::exception
{
  //! An error message passed on initialization
  std::string int_msg;
  const char * what_str;

  //! Create a Mongo Exception, and store the given error message
  MongoException (std::string msg) {int_msg = "Error in Mongo Request: " + msg;what_str = int_msg.c_str();}

  MongoException () {}
  ~MongoException() throw () {}

  //! Show the error message in readable format
  const char * what() const throw ()
  {
    return what_str;
  }
};

//! Interface used to store Mongo Responses
class MongoResponseInterface {
public:
  virtual ~MongoResponseInterface() {}
  //! Retrieve the value stored inside the response interface
  virtual std::string get_value() = 0;
};

//! Returned from Queries in order to iterate over results
class MongoIteratorInterface {
public:
  virtual ~MongoIteratorInterface() {}
  //! Get the next value from the iterator.

  //! Will return a Mongo Response Interface when available,
  //! Otherwise will return NULL.
  virtual MongoResponseInterface* next() = 0;
};

class MongoInterface {
public:

  //Destructor
  virtual ~MongoInterface() {}

  //CRUD Operations

  //! Create JSON Document, returns the document key
  virtual MongoResponseInterface* create_document(const char * doc, const char * collection_name) = 0;
  //! Create JSON Document, returns the document key
  virtual MongoResponseInterface* create_document(const char * doc) = 0;
  //! Create JSON Document, returns the document key
  virtual MongoResponseInterface* create_document(std::string doc) = 0;
  //! Create JSON Document, returns the document key
  virtual MongoResponseInterface* create_document(std::string doc, std::string collection_name) = 0;

  //! Delete a JSON Document, returns true if successful
  virtual void delete_document(const char * key) = 0;
  //! Delete a JSON Document, returns true if successful
  virtual void delete_document(std::string key) = 0;
  //! Delete a JSON Document, returns true if successful
  virtual void delete_document(const char * key, const char * collection_name) = 0;
  //! Delete a JSON Document, returns true if successful
  virtual void delete_document(std::string key, std::string collection_name) = 0;

  //! Retrieve a JSON Document and return it in a std::string
  virtual MongoResponseInterface* load_document(const char * key) = 0;
  //! Retrieve a JSON Document and return it in a std::string
  virtual MongoResponseInterface* load_document(std::string key) = 0;
  //! Retrieve a JSON Document and return it in a std::string
  virtual MongoResponseInterface* load_document(const char * key, const char * collection_name) = 0;
  //! Retrieve a JSON Document and return it in a std::string
  virtual MongoResponseInterface* load_document(std::string key, std::string collection_name) = 0;

  //! Update an existing document, returns true if successful
  virtual void save_document(const char * doc, const char * key) = 0;
  //! Update an existing document, returns true if successful
  virtual void save_document(std::string doc, std::string key) = 0;
  //! Update an existing document, returns true if successful
  virtual void save_document(const char * doc, const char * key, const char * collection_name) = 0;
  //! Update an existing document, returns true if successful
  virtual void save_document(std::string doc, std::string key, std::string collection_name) = 0;

  //Advanced Operations

  //! Queries

  //! Accept the query and query options in JSON format.
  //! Return an iterator which can be used to access query results
  virtual MongoIteratorInterface* query(const char * query_str, const char * opts_str) = 0;
  //! Queries

  //! Accept the query and query options in JSON format.
  //! Return an iterator which can be used to access query results
  virtual MongoIteratorInterface* query(std::string query_str, std::string opts_str) = 0;
  //! Queries

  //! Accept the query and query options in JSON format.
  //! Return an iterator which can be used to access query results
  virtual MongoIteratorInterface* query(const char * query_str, const char * opts_str, const char * collection_name) = 0;
  //! Queries

  //! Accept the query and query options in JSON format.
  //! Return an iterator which can be used to access query results
  virtual MongoIteratorInterface* query(std::string query_str, std::string opts_str, std::string collection_name) = 0;
  //! Queries

  //! Accept the query in JSON format.
  //! Return an iterator which can be used to access query results
  virtual MongoIteratorInterface* query(const char * query_str) = 0;
  //! Queries

  //! Accept the query in JSON format.
  //! Return an iterator which can be used to access query results
  virtual MongoIteratorInterface* query(std::string query_str) = 0;

};

#endif
