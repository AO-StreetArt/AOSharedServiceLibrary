#include <string>
#include <exception>
#include <vector>

#ifndef AOSSL_MONGO_INTERFACE
#define AOSSL_MONGO_INTERFACE

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
