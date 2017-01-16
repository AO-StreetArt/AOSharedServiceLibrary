# Mongo Interface

Mongo is a document-based database that allows for persistent, clusterable storage of data objects
for services.

We can start by including the necessary headers

    #include "include/factory/mongo_interface.h"
    #include "include/factory_mongo.h"

While Mongo utilizes a special brand of json known as 'BSON', the interface will take care of the translation.
This means we can define our objects as json, and then feed them back to the interface.

    std::string json = "{\"name\": {\"first\":\"Grace\", \"last\":\"Hopper\"}}";
    std::string json2 = "{\"name\": {\"first\":\"Alex\", \"last\":\"Barry\"}}";

    const char *json3 = "{\"name\": {\"first\":\"Keasha\", \"last\":\"Norton\"}, \"number\": 1}";
    const char *json4 = "{\"name\": {\"first\":\"Gummy\", \"last\":\"Bear\"}, \"number\": 1}";

    std::string key1;
    std::string key2;
    std::string key3;
    std::string key4;
    bool success;

    MongoComponentFactory mongo_factory;
    MongoInterface *mongo = mongo_factory.get_mongo_interface("mongodb://localhost:27017/", "mydb", "mycoll");

We can create a new object in the DB

    key1 = mongo->create_document(json);

    if ( !(key1.empty()) ) {
      std::cout << "Document written to Mongo DB" << std::endl;
    }

We can load an existing object from the DB

    std::string json_doc = mongo->load_document(key1);

    std::cout << "Document Loaded from Mongo: " << json_doc << std::endl;

Update an existing document

    success = mongo->save_document(json2, key1);

    if (success) {
      std::cout << "Document written to Mongo DB" << std::endl;
    }

Delete a document from the DB

    success = mongo->delete_document(key1);

    if (success) {
      std::cout << "Document deleted from Mongo DB" << std::endl;
    }

We can change the current collection using the collection name in an operation

    key2 = mongo->create_document(json3, "newcoll");

Once we've set the current collection, we can continue to access that collection in the
same manner as before

    key3 = mongo->create_document(json4);

We also have the ability to run queries against the DB

    std::vector<std::string> query_response;
    std::string query_string1 = "{\"number\": 1}";

    std::cout << "Executing Query:" << query_string1 << std::endl;
    query_response = mongo->query(query_string1);
    std::cout << query_response[0] << " " << query_response[1] << std::endl;
