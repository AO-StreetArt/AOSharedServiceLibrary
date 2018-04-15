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

#include <assert.h>
#include <iostream>
#include <string>
#include "include/mongo_interface.h"
#include "include/factory_mongo.h"
#include "include/mongo_buffer.h"

int print_iterator(MongoIteratorInterface *iter) {
  int num_iterations = 0;
  MongoResponseInterface *resp = iter->next();
  while (resp) {
    if (!(resp->get_err_msg().empty())) break;
    num_iterations = num_iterations + 1;
    std::cout << resp->get_value() << std::endl;
    delete resp;
    resp = iter->next();
  }
  return num_iterations;
}

void validate_document_fields() {

}

void test_mongo_buffer() {
  AOSSL::MongoBuffer *buf = new AOSSL::MongoBuffer;
  std::string dkey1 = "name";
  std::string dkey2 = "first";
  std::string dkey3 = "last";
  std::string dkey4 = "numbers";
  std::string dkey5 = "integers";
  std::string dkey6 = "double";
  std::string dkey7 = "date";
  std::string search_key = "name.last";
  std::string dval1 = "Alex";
  std::string dval2 = "Barry";
  buf->add_string(dkey2, dval1);
  assert(buf->has_field(dkey2));
  assert(!(buf->has_field(dkey1)));
  buf->add_int(dkey4, 1);
  assert(buf->has_field(dkey4));
  buf->add_double(dkey6, 1.01);
  assert(buf->has_field(dkey6));
  assert(buf->count_keys() == 3);
  buf->start_object(dkey1);
  buf->add_string(dkey3, dval2);
  buf->end_object();
  assert(buf->has_field(dkey1));
  assert(buf->has_field(search_key));
  assert(buf->count_keys() == 4);
  buf->start_array(dkey5);
  buf->add_int(1);
  buf->add_int(2);
  buf->end_array();
  assert(buf->has_field(dkey5));
  assert(buf->count_keys() == 5);
  buf->add_date(dkey7, 1522515115);
  assert(buf->has_field(dkey7));
  assert(buf->count_keys() == 6);
  std::cout << "Test Mongo Buffer:" << std::endl;
  std::cout << buf->to_json() << std::endl;
  delete buf;
}

void test_bson_api(MongoInterface *mongo, MongoInterface *bad_mongo) {
  std::string key1;
  std::string json_doc;
  std::string json_doc2;
  MongoResponseInterface *resp1 = NULL;
  MongoResponseInterface *resp2 = NULL;
  MongoResponseInterface *resp3 = NULL;
  // Creation Test
  std::cout << "Writing Document to Mongo DB" << std::endl;
  AOSSL::MongoBuffer *buf = new AOSSL::MongoBuffer;
  std::string dkey1 = "name";
  std::string dkey2 = "first";
  std::string dkey3 = "last";
  std::string dkey4 = "numbers";
  std::string dkey5 = "integers";
  std::string dkey6 = "double";
  std::string dkey7 = "date";
  std::string dval1 = "Alex";
  std::string dval2 = "Barry";
  buf->start_object(dkey1);
  buf->add_string(dkey2, dval1);
  buf->add_string(dkey3, dval2);
  buf->end_object();
  buf->start_object(dkey4);
  buf->start_array(dkey5);
  buf->add_int(1);
  buf->add_int(2);
  buf->end_array();
  buf->add_double(dkey6, 1.01);
  buf->end_object();
  buf->add_date(dkey7, 1522515115);
  try {
    resp1 = mongo->create_document(buf);
    key1 = resp1->get_value();
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    assert(false);
  }
  std::cout << "Document written to Mongo DB: " << key1 << std::endl;
  if (resp1) {
    assert(!(key1.empty()));
    delete resp1;
  }
  // Load Test
  try {
    resp2 = mongo->load_document(key1);
    json_doc = resp2->get_value();
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    assert(false);
  }

  std::cout << "Document loaded from Mongo: " << json_doc << std::endl;
  assert(!(json_doc.empty()));

  // Update test
  AOSSL::MongoBuffer *update_buf = new AOSSL::MongoBuffer;
  std::string uval1 = "Betty";
  std::string uval2 = "Bear";
  update_buf->start_object(dkey1);
  update_buf->add_string(dkey2, uval1);
  update_buf->add_string(dkey3, uval2);
  update_buf->end_object();
  update_buf->start_object(dkey4);
  update_buf->start_array(dkey5);
  update_buf->add_int(3);
  update_buf->add_int(4);
  update_buf->end_array();
  update_buf->add_double(dkey6, 2.56);
  update_buf->end_object();

  try {
    mongo->save_document(update_buf, key1);
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    assert(false);
  }

  // Load Test
  try {
    resp3 = mongo->load_document(key1);
    json_doc2 = resp3->get_value();
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    assert(false);
  }

  std::cout << "Document loaded from Mongo: " << json_doc2 << std::endl;
  assert(!(json_doc2.empty()));
  // Compare the loaded object after creation with the loaded object after
  // update.  If they are the same, then update didn't persist
  std::cout << json_doc << std::endl;
  std::cout << json_doc2 << std::endl;
  assert(!(json_doc.compare(json_doc2) == 0));
  if (resp2) {
    delete resp2;
  }
  if (resp3) {
    delete resp3;
  }

  // Update by Query Test
  AOSSL::MongoBuffer *update_buffer = new AOSSL::MongoBuffer;
  std::string update_op_key = "$set";
  std::string update_key = "name.first";
  std::string first_name = "JellyBean";
  update_buffer->start_object(update_op_key);
  update_buffer->add_string(update_key, first_name);
  update_buffer->end_object();

  AOSSL::MongoBuffer *query_buffer = new AOSSL::MongoBuffer;
  std::string query_key = "name.first";
  std::string query_val = "Alex";
  query_buffer->add_string(query_key, query_val);

  try {
    mongo->update_by_query(query_buffer, update_buffer);
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    assert(false);
  }

  std::cout << "Update by query made successfully" << std::endl;

  // Update Single by query test
  AOSSL::MongoBuffer *supdate_buffer = new AOSSL::MongoBuffer;
  std::string supdate_op_key = "$set";
  std::string supdate_key = "name.first";
  std::string sfirst_name = "GummyBear";
  supdate_buffer->start_object(supdate_op_key);
  supdate_buffer->add_string(supdate_key, sfirst_name);
  supdate_buffer->end_object();

  AOSSL::MongoBuffer *squery_buffer = new AOSSL::MongoBuffer;
  std::string squery_key = "name.first";
  std::string squery_val = "JellyBean";
  squery_buffer->add_string(query_key, query_val);

  AOSSL::MongoBufferInterface *upd_response = NULL;

  try {
    upd_response = mongo->update_single_by_query(squery_buffer, supdate_buffer);
    std::cout << upd_response->to_json() << std::endl;
    std::string mdc_key = "modifiedCount";
    std::string mtc_key = "matchedCount";
    std::string mdc_val;
    std::string mtc_val;
    std::cout << upd_response->get_int(mdc_key) << std::endl;
    std::cout << upd_response->get_int(mtc_key) << std::endl;
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    assert(false);
  }

  // Load Test
  try {
    resp2 = mongo->load_document(key1);
    json_doc = resp2->get_value();
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    assert(false);
  }

  std::cout << "Document bufLoaded from Mongo: " << json_doc << std::endl;
  assert(!(json_doc.empty()));
  if (resp2) {
    delete resp2;
  }

  // Delete Test
  try {
    mongo->delete_document(key1);
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    assert(false);
  }

  std::cout << "Document deleted from Mongo DB" << std::endl;

  delete buf;
  delete update_buffer;
  delete query_buffer;
  delete supdate_buffer;
  delete squery_buffer;
  delete update_buf;
  if (upd_response) delete upd_response;
}

void test_json_api(MongoInterface *mongo, MongoInterface *bad_mongo) {
  const char *json = "{\"name\": {\"first\":\"Grace\", \"last\":\"Hopper\"}}";
  const char *json2 = "{\"name\": {\"first\":\"Alex\", \"last\":\"Barry\"}}";
  const char *json3 = \
  "{\"name\": {\"first\":\"Keasha\", \"last\":\"Norton\"}, \"number\": 1}";
  const char *json4 = \
  "{\"name\": {\"first\":\"Gummy\", \"last\":\"Bear\"}, \"number\": 1}";
  const char *json5 = \
  "{\"name\": {\"first\":\"Keasha\", \"last\":\"Norton\"}, \"number\": 2}";

  std::string key1;
  std::string key2;
  std::string key3;
  std::string key4;
  std::string json_doc;

  MongoResponseInterface *resp1 = NULL;
  MongoResponseInterface *resp2 = NULL;
  MongoResponseInterface *resp3 = NULL;
  MongoResponseInterface *resp4 = NULL;
  MongoResponseInterface *resp5 = NULL;
  MongoResponseInterface *resp6 = NULL;
  MongoResponseInterface *resp7 = NULL;
  MongoResponseInterface *resp8 = NULL;
  MongoResponseInterface *resp9 = NULL;
  MongoResponseInterface *resp10 = NULL;
  MongoResponseInterface *resp11 = NULL;

  MongoIteratorInterface *iter1 = NULL;
  MongoIteratorInterface *iter2 = NULL;

  // Creation Test
  std::cout << "Writing Document to Mongo DB" << std::endl;

  try {
    resp1 = mongo->create_document(json);
    key1 = resp1->get_value();
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    assert(false);
  }

  std::cout << "Document written to Mongo DB: " << key1 << std::endl;
  if (resp1) {
    delete resp1;
  }

  // Load Test
  try {
    resp2 = mongo->load_document(key1);
    json_doc = resp2->get_value();
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    assert(false);
  }

  std::cout << "Document Loaded from Mongo: " << json_doc << std::endl;
  assert(!(json_doc.empty()));
  if (resp2) {
    delete resp2;
  }

  // Update Test
  try {
    mongo->save_document(json2, key1);
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    assert(false);
  }

  std::cout << "Document written to Mongo DB" << std::endl;

  json_doc.clear();
  try {
    resp3 = mongo->load_document(key1);
    json_doc = resp3->get_value();
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    assert(false);
  }

  std::cout << "Document Loaded from Mongo: " << json_doc << std::endl;
  assert(!(json_doc.empty()));
  if (resp3) {
    delete resp3;
  }

  // Delete Test
  try {
    mongo->delete_document(key1);
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    assert(false);
  }

  std::cout << "Document deleted from Mongo DB" << std::endl;

  json_doc.clear();
  try {
    resp4 = mongo->load_document(key1);
    assert(!resp4);
    // json_doc = resp4->get_value();
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    assert(false);
  }
  assert(json_doc.empty());

  std::cout << "Document Loaded from Mongo: " << json_doc << std::endl;

  if (resp4) {
    delete resp4;
  }

  // Secondary Collection tests
  try {
    resp5 = mongo->create_document(json3, "newcoll");
    key2 = resp5->get_value();
    resp6 = mongo->create_document(json4);
    key3 = resp6->get_value();
    resp7 = mongo->create_document(json5);
    key4 = resp7->get_value();
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    assert(false);
  }

  std::cout << "Documents Created: " << key2 << ", " << \
  key3 << ", " << key4 << std::endl;
  assert(!(key2.empty()));
  assert(!(key3.empty()));
  assert(!(key4.empty()));
  if (resp5) {
    delete resp5;
  }
  if (resp6) {
    delete resp6;
  }
  if (resp7) {
    delete resp7;
  }

  json_doc.clear();
  try {
    resp8 = mongo->load_document(key2);
    json_doc = resp8->get_value();
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    assert(false);
  }

  std::cout << "Document Loaded from Mongo: " << json_doc << std::endl;

  if (resp8) {
    delete resp8;
  }

  json_doc.clear();
  try {
    resp9 = mongo->load_document(key3);
    json_doc = resp9->get_value();
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    assert(false);
  }

  std::cout << "Document Loaded from Mongo: " << json_doc << std::endl;

  if (resp9) {
    delete resp9;
  }

  // Query Tests
  std::string query_string1 = "{\"number\": 1}";
  std::string query_string2 = \
  "{\"name\": {\"first\":\"Keasha\", \"last\":\"Norton\"}}";

  std::cout << "Executing Query:" << query_string1 << std::endl;
  try {
    iter1 = mongo->query(query_string1);
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    assert(false);
  }
  if (iter1) {
    assert(print_iterator(iter1) == 2);
    delete iter1;
  }

  std::cout << "Executing Query:" << query_string2 << std::endl;
  try {
    iter2 = mongo->query(query_string2);
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    assert(false);
  }
  if (iter2) {
    assert(print_iterator(iter2) == 2);
    delete iter2;
  }

  try {
    mongo->delete_document(key2);
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    assert(false);
  }

  try {
    mongo->delete_document(key3);
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    assert(false);
  }

  try {
    mongo->delete_document(key4);
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    assert(false);
  }

  // Failure Tests

    key1 = "";

    try {
      resp10 = bad_mongo->create_document(json);
      key1 = resp10->get_value();
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }

    if (!(key1.empty())) {
      std::cout << "Document written to Mongo DB" << std::endl;
      std::cout << key1 << std::endl;
      if (resp10) {delete resp10;}
      assert(false);
    }
    if (resp10) {delete resp10;}

    key1 = "abcdef";

    // Load Test
    std::string json_doc_fail = "";
    try {
      resp11 = mongo->load_document(key1);
      if (resp11) {
        json_doc_fail = resp11->get_value();
      }
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }

    std::cout << "Document Loaded from Mongo: " << json_doc_fail << std::endl;
    if (!(json_doc_fail.empty())) {
      if (resp11) {delete resp11;}
      assert(false);
    }
    if (resp11) {delete resp11;}

    // Update Test
    try {
      bad_mongo->save_document(json2, key1);
      assert(false);
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }

    // Delete Test
    try {
      bad_mongo->delete_document(key1);
      assert(false);
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
}

int main(int argc, char *argv[]) {
    MongoComponentFactory mongo_factory;
    MongoInterface *mongo = mongo_factory.get_mongo_interface(\
      "mongodb://localhost:27017/", "mydb", "mycoll");
    MongoInterface *bad_mongo = mongo_factory.get_mongo_interface(\
      "mongodb://localhost:27018/", "mydb", "mycoll");

    // Run Tests
    try {
      test_json_api(mongo, bad_mongo);
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }

    try {
      test_bson_api(mongo, bad_mongo);
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }

    try {
      test_mongo_buffer();
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }

    if (mongo) {
      delete mongo;
    }
    if (bad_mongo) {
      delete bad_mongo;
    }

    return 0;
  }
