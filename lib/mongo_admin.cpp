#include "include/mongo_admin.h"

void MongoClient::initialize(const char * url, const char * db, const char * collection_name)
{
  //Initialize the Mongo C-Driver
  mongoc_init ();

  //Connect to the Mongo Instance
  client = mongoc_client_new (url);

  //Get a collection from Mongo
  collection = mongoc_client_get_collection (client, db, collection_name);

  //Set up the current DB Name
  db_name = db;
}

MongoClient::~MongoClient()
{
  mongoc_collection_destroy (collection);
  mongoc_client_destroy (client);
  mongoc_cleanup ();
}

std::string MongoClient::create_document(const char * document)
{
bson_t *doc = NULL;
bson_error_t error;
bson_oid_t oid;
std::string new_key;

//Perform the conversion
doc = bson_new_from_json ((const uint8_t *)document, -1, &error);

//Check if the conversion succeeded
bool success = false;
if (!doc) {
  std::string err1 = "Error Converting JSON: ";
  std::string err2 (error.message);
  throw MongoException(err1+err2);
}
else {
  //Append the key
  char * c_str = new char[25];
  bson_oid_init (&oid, NULL);
  BSON_APPEND_OID (doc, "_id", &oid);
  bson_oid_to_string(&oid, c_str);
  new_key.assign(c_str, 25);

  //Actually insert the document
  success = mongoc_collection_insert (collection, MONGOC_INSERT_NONE, doc, NULL, &error);
}

//Destroy the bson document
bson_destroy (doc);

if ( success && !(new_key.empty()) ) {
  return new_key;
}
else if ( !(success) ) {
  throw MongoException(error.message);
  return "";
}
else {
  return "";
}

}

bool MongoClient::save_document(const char * document, const char * key)
{
bson_t *doc = NULL;
bson_error_t error;
bson_oid_t oid;

//Perform the conversion
doc = bson_new_from_json ((const uint8_t *)document, -1, &error);

//Check if the conversion succeeded
bool success = false;
if (!doc) {
  std::string err1 = "Error Converting JSON: ";
  std::string err2 (error.message);
  throw MongoException(err1+err2);
  }
  else {
    //Append the key
    bson_oid_init_from_string (&oid, key);
    BSON_APPEND_OID (doc, "_id", &oid);

    //Actually insert the document
    success = mongoc_collection_save (collection, doc, NULL, &error);
  }
  //Destroy the bson document
  bson_destroy (doc);
  if (!success) {
    throw MongoException(error.message);
  }
  return success;
}

bool MongoClient::delete_document(const char * key)
{
//Setup necessary variables
bson_t *doc = NULL;
bson_error_t error;
bson_oid_t oid;

//Write a new bson query with the key provided
doc = bson_new();
bson_oid_init_from_string (&oid, key);
BSON_APPEND_OID (doc, "_id", &oid);

//Execute the query
bool success = mongoc_collection_remove(collection, MONGOC_REMOVE_SINGLE_REMOVE, doc, NULL, &error);

bson_destroy(doc);

return success;
}

void MongoClient::switch_collection(const char * collection_name)
{
//Disconnect from the old collection
mongoc_collection_destroy (collection);

//Connect to the new collection
collection = mongoc_client_get_collection (client, db_name, collection_name);
}

std::string MongoClient::load_document(const char * key)
{
//Setup necessary variables
const bson_t *doc = NULL;
bson_t *query = NULL;
bson_oid_t oid;
mongoc_cursor_t *cursor = NULL;
char * str;

//Write a new bson query with the key provided
query = bson_new();
bson_oid_init_from_string (&oid, key);
BSON_APPEND_OID (query, "_id", &oid);

cursor = mongoc_collection_find (collection, MONGOC_QUERY_NONE, 0, 0, 0, query, NULL, NULL);

std::vector<std::string> ret_vals;

//Cycle through the Mongo Cursor for all documents returned
while (mongoc_cursor_next (cursor, &doc)) {
  str = bson_as_json (doc, NULL);
  std::string ret_val (str);
  ret_vals.push_back(ret_val);
  bson_free (str);
}

bson_destroy (query);
mongoc_cursor_destroy (cursor);

//Set up the return value
int num_docs = ret_vals.size();

if (num_docs > 1) {
  throw MongoException("Multiple Documents returned on key");
}
else if (num_docs < 1) {
  return "";
}
else {
  return ret_vals[0];
}

}

std::vector<std::string> MongoClient::query(const char * query_str)
{
//Setup necessary variables
const bson_t *doc = NULL;
bson_t *q = NULL;
bson_error_t error;
mongoc_cursor_t *cursor = NULL;
char * str;

//Write a new bson query with the key provided
q = bson_new_from_json ((const uint8_t *)query_str, -1, &error);

cursor = mongoc_collection_find (collection, MONGOC_QUERY_NONE, 0, 0, 0, q, NULL, NULL);

std::vector<std::string> ret_vals;

//Cycle through the Mongo Cursor for all documents returned
while (mongoc_cursor_next (cursor, &doc)) {
  str = bson_as_json (doc, NULL);
  std::string ret_val (str);
  ret_vals.push_back(ret_val);
  bson_free (str);
}

bson_destroy (q);
mongoc_cursor_destroy (cursor);

return ret_vals;
}
