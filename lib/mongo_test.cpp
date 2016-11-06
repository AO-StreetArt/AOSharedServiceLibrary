#include "include/factory/mongo_interface.h"
#include "include/factory_mongo.h"
#include <iostream>
#include <assert.h>

void print_vector(std::vector<std::string> inp) {
  for (int i = 0; i < inp.size(); i++) {
    std::cout << inp[i] << std::endl;
  }
}

int
main (int   argc,
      char *argv[])
{
  const char *json = "{\"name\": {\"first\":\"Grace\", \"last\":\"Hopper\"}}";
  const char *json2 = "{\"name\": {\"first\":\"Alex\", \"last\":\"Barry\"}}";

  const char *json3 = "{\"name\": {\"first\":\"Keasha\", \"last\":\"Norton\"}, \"number\": 1}";
  const char *json4 = "{\"name\": {\"first\":\"Gummy\", \"last\":\"Bear\"}, \"number\": 1}";
  const char *json5 = "{\"name\": {\"first\":\"Keasha\", \"last\":\"Norton\"}, \"number\": 2}";

  std::string key1;
  std::string key2;
  std::string key3;
  std::string key4;
  bool success;

  MongoComponentFactory mongo_factory;
  MongoInterface *mongo = mongo_factory.get_mongo_interface("mongodb://localhost:27017/", "mydb", "mycoll");

  //Creation Test
  std::cout << "Writing Document to Mongo DB" << std::endl;

  key1 = mongo->create_document(json);

  if ( !(key1.empty()) ) {
    std::cout << "Document written to Mongo DB" << std::endl;
  }
  else {
    std::cout << "Error writing document to Mongo DB" << std::endl;
    assert( false );
  }

  //Load Test
  std::string json_doc = mongo->load_document(key1);

  std::cout << "Document Loaded from Mongo: " << json_doc << std::endl;
  assert( !(json_doc.empty()) );

  //Update Test
  success = mongo->save_document(json2, key1);

  if (success) {
    std::cout << "Document written to Mongo DB" << std::endl;
  }
  else {
    std::cout << "Error writing document to Mongo DB" << std::endl;
    assert( success );
  }

  json_doc.clear();
  json_doc = mongo->load_document(key1);

  std::cout << "Document Loaded from Mongo: " << json_doc << std::endl;
  assert( !(json_doc.empty()) );

  //Delete Test
  success = mongo->delete_document(key1);

  if (success) {
    std::cout << "Document deleted from Mongo DB" << std::endl;
  }
  else {
    std::cout << "Error deleting document from Mongo DB" << std::endl;
    assert(success);
  }

  json_doc.clear();
  json_doc = mongo->load_document(key1);
  assert( json_doc.empty() );

  std::cout << "Document Loaded from Mongo: " << json_doc << std::endl;

  //Secondary Collection tests
  key2 = mongo->create_document(json3, "newcoll");
  key3 = mongo->create_document(json4);
  key4 = mongo->create_document(json5);

  if ( !(key2.empty() || key3.empty()) ) {
    std::cout << "Document written to Mongo DB" << std::endl;
  }
  else {
    std::cout << "Error writing document to Mongo DB" << std::endl;
  }

  json_doc.clear();
  json_doc = mongo->load_document(key2);

  std::cout << "Document Loaded from Mongo: " << json_doc << std::endl;

  json_doc.clear();
  json_doc = mongo->load_document(key3);

  std::cout << "Document Loaded from Mongo: " << json_doc << std::endl;

  //Query Tests
  std::vector<std::string> query_response;
  std::string query_string1 = "{\"number\": 1}";
  std::string query_string2 = "{\"name\": {\"first\":\"Keasha\", \"last\":\"Norton\"}}";

  std::cout << "Executing Query:" << query_string1 << std::endl;
  query_response = mongo->query(query_string1);
  print_vector(query_response);
  assert(query_response.size() == 2);

  query_response.clear();

  std::cout << "Executing Query:" << query_string2 << std::endl;
  query_response = mongo->query(query_string2);
  print_vector(query_response);
  assert(query_response.size() == 2);

  success = mongo->delete_document(key2);

  if (success) {
    std::cout << "Document deleted from Mongo DB" << std::endl;
  }
  else {
    std::cout << "Error deleting document from Mongo DB" << std::endl;
  }

  success = mongo->delete_document(key3);

  if (success) {
    std::cout << "Document deleted from Mongo DB" << std::endl;
  }
  else {
    std::cout << "Error deleting document from Mongo DB" << std::endl;
  }

  success = mongo->delete_document(key4);

  if (success) {
    std::cout << "Document deleted from Mongo DB" << std::endl;
  }
  else {
    std::cout << "Error deleting document from Mongo DB" << std::endl;
  }

  return 0;
}
