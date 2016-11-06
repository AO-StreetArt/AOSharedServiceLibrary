#include <bson.h>
#include <mongoc.h>
#include <string>
#include <exception>
#include <vector>

#include "factory/mongo_interface.h"

//Stable, C++11 Mongo Client

#ifndef AOSSL_MONGO_CLIENT
#define AOSSL_MONGO_CLIENT

class MongoClient: public MongoInterface {

//The internal mongoc client
mongoc_client_t *client;

//The current DB
const char * db_name;

//The current collection
mongoc_collection_t *collection;

//Initialize the client
void initialize(const char * url, const char * db, const char * collection_name);

public:

  //Switch the current collection
  void switch_collection(const char * collection_name);
  void switch_collection(std::string collection_name) {switch_collection(collection_name.c_str());}

  //Constructor
  MongoClient(const char * url, const char * db, const char * collection_name) {initialize(url, db, collection_name);}
  MongoClient(std::string url, std::string db, std::string collection_name) {initialize(url.c_str(), db.c_str(), collection_name.c_str());}
  MongoClient(const char * url, const char * db) {initialize(url, db, "default");}
  MongoClient(std::string url, std::string db) {initialize(url.c_str(), db.c_str(), "default");}

  //Destructor
  ~MongoClient();

  //CRUD Operations

  //Create JSON Document
  //Returns the document key
  std::string create_document(const char * doc);
  std::string create_document(std::string doc) {return create_document(doc.c_str());}
  std::string create_document(const char * doc, const char * collection_name) {switch_collection(collection_name);return create_document(doc);}
  std::string create_document(std::string doc, std::string collection_name) {switch_collection(collection_name);return create_document(doc.c_str());}

  //Delete a JSON Document
  bool delete_document(const char * key);
  bool delete_document(std::string key) {return delete_document(key.c_str());}
  bool delete_document(const char * key, const char * collection_name) {switch_collection(collection_name);return delete_document(key);}
  bool delete_document(std::string key, std::string collection_name) {return delete_document(key.c_str(), collection_name.c_str());}

  //Retrieve a JSON Document
  std::string load_document(const char * key);
  std::string load_document(std::string key) {return load_document(key.c_str());}
  std::string load_document(const char * key, const char * collection_name) {switch_collection(collection_name);return load_document(key);}
  std::string load_document(std::string key, std::string collection_name){return load_document(key.c_str(), collection_name.c_str());}

  //Save Document
  //If the document has an '_id' field it will be updated.  Otherwise it will be inserted
  bool save_document(const char * doc, const char * key);
  bool save_document(std::string doc, std::string key) {return save_document(doc.c_str(), key.c_str());}
  bool save_document(const char * doc, const char * key, const char * collection_name) {switch_collection(collection_name);return save_document(doc, key);}
  bool save_document(std::string doc, std::string key, std::string collection_name) {return save_document(doc.c_str(), key.c_str(), collection_name.c_str());}

  //Advanced Operations

  //Queries, accept the query in JSON format
  std::vector<std::string> query(const char * query_str);
  std::vector<std::string> query(std::string query_str) {return query(query_str.c_str());}
  std::vector<std::string> query(const char * query_str, const char * collection_name) {switch_collection(collection_name); return query(query_str);}
  std::vector<std::string> query(std::string query_str, std::string collection_name) {switch_collection(collection_name); return query(query_str.c_str());}

};

#endif
