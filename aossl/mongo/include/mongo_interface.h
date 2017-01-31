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

struct MongoException: public std::exception
{
  //! An error message passed on initialization
  std::string int_msg;

  //! Create a Mongo Exception, and store the given error message
  MongoException (std::string msg) {int_msg = msg;}

  MongoException () {}
  ~MongoException() throw () {}

  //! Show the error message in readable format
  const char * what() const throw ()
  {
    std::string what_str = "Error in Mongo Request: " + int_msg;
    return what_str.c_str();
  }
};

class MongoInterface {
public:

  //Destructor
  virtual ~MongoInterface() {}

  //CRUD Operations

  //! Create JSON Document, returns the document key
  virtual std::string create_document(const char * doc) = 0;
  //! Create JSON Document, returns the document key
  virtual std::string create_document(std::string doc) = 0;
  //! Create JSON Document, returns the document key
  virtual std::string create_document(const char * doc, const char * collection_name) = 0;
  //! Create JSON Document, returns the document key
  virtual std::string create_document(std::string doc, std::string collection_name) = 0;

  //! Delete a JSON Document, returns true if successful
  virtual bool delete_document(const char * key) = 0;
  //! Delete a JSON Document, returns true if successful
  virtual bool delete_document(std::string key) = 0;
  //! Delete a JSON Document, returns true if successful
  virtual bool delete_document(const char * key, const char * collection_name) = 0;
  //! Delete a JSON Document, returns true if successful
  virtual bool delete_document(std::string key, std::string collection_name) = 0;

  //! Retrieve a JSON Document and return it in a std::string
  virtual std::string load_document(const char * key) = 0;
  //! Retrieve a JSON Document and return it in a std::string
  virtual std::string load_document(std::string key) = 0;
  //! Retrieve a JSON Document and return it in a std::string
  virtual std::string load_document(const char * key, const char * collection_name) = 0;
  //! Retrieve a JSON Document and return it in a std::string
  virtual std::string load_document(std::string key, std::string collection_name) = 0;

  //! Update an existing document, returns true if successful
  virtual bool save_document(const char * doc, const char * key) = 0;
  //! Update an existing document, returns true if successful
  virtual bool save_document(std::string doc, std::string key) = 0;
  //! Update an existing document, returns true if successful
  virtual bool save_document(const char * doc, const char * key, const char * collection_name) = 0;
  //! Update an existing document, returns true if successful
  virtual bool save_document(std::string doc, std::string key, std::string collection_name) = 0;

  //Advanced Operations

  //! Queries

  //! Accept the query in JSON format, and
  //! Return a vector of documents as std::string objects
  virtual std::vector<std::string> query(const char * query_str) = 0;
  //! Queries

  //! Accept the query in JSON format, and
  //! Return a vector of documents as std::string objects
  virtual std::vector<std::string> query(std::string query_str) = 0;
  //! Queries

  //! Accept the query in JSON format, and
  //! Return a vector of documents as std::string objects
  virtual std::vector<std::string> query(const char * query_str, const char * collection_name) = 0;
  //! Queries

  //! Accept the query in JSON format, and
  //! Return a vector of documents as std::string objects
  virtual std::vector<std::string> query(std::string query_str, std::string collection_name) = 0;

};

#endif
