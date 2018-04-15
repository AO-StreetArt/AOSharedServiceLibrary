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

#ifndef AOSSL_MONGO_INCLUDE_MONGO_BUFFER_H_
#define AOSSL_MONGO_INCLUDE_MONGO_BUFFER_H_

#include <bson.h>
#include <mongoc.h>
#include <exception>
#include <string>
#include <unordered_map>

#include "aossl/core/include/buffers.h"
#include "mongo_buffer_interface.h"

namespace AOSSL {

//! A Structure for holding a BSON Document
class MongoBuffer: public Buffer, public MongoBufferInterface {
 private:
  std::vector<bson_t*> children;
  int parent_index = 0;
  int child_index = 0;
  std::vector<int> array_indices;
  char *json_cstring = NULL;
  std::string json_string;
  bson_oid_t oid;
  bson_iter_t iterator;
  bool iter_initialized = false;
  inline void find_element(std::string key) {
    if (!iter_initialized) {
      bson_iter_init(&iterator, children[0]);
      iter_initialized = true;
    }
    bson_iter_find(&iterator, key.c_str());
    bson_type_t elt_type = bson_iter_type(&iterator);
    if (elt_type == BSON_TYPE_EOD) {throw BsonException("Element not found");}
  }
 public:
  //! Build a new Mongo Buffer
  MongoBuffer() {bson_t *b = bson_new(); children.push_back(b);}
  //! Build a new Mongo Buffer
  MongoBuffer(bson_t *b) {children.push_back(b);}
  //! Destroy the Mongo Buffer
  ~MongoBuffer() {if (json_cstring) {bson_free(json_cstring); json_cstring = NULL;} bson_destroy(children[0]); for (unsigned int i = 1; i < children.size(); i++) {delete children[i];}}
  //! Is a successful response
  bool successful() {return Buffer::success;}
  //! Error message
  std::string get_error_message() {return Buffer::err_msg;}
  //! Get the underlying bson_t document to pass to the mongoc driver
  bson_t* get_bson() {return children[0];}
  inline void add_oid(std::string value) {
    // Copy our key into a character array in a memory safe manner
    // Mongo OID will be returned as 24 bit hex-encoded string
    char key_str[25];
    strncpy(key_str, value.c_str(), 25);
    key_str[24] = '\0';
    // Copy the character array into the OID
    bson_oid_init_from_string(&oid, key_str);
    // Append the OID to the root of the document
    BSON_APPEND_OID(children[0], "_id", &oid);
  }
  inline void get_oid(std::string key, std::string &value) {
    find_element(key);
    const bson_oid_t *oid = bson_iter_oid(&iterator);
    char str[25];
    bson_oid_to_string(oid, str);
    value.assign(str);
  }
  //! Add a string value to the buffer
  void add_string(std::string key, std::string value) {add_string(key, value, MONGO_STRING_UTF8);}
  //! Add a string value to the open array in the buffer
  inline void add_string(std::string value, int encoding) {
    char str[16];
    const char *key;
    bson_uint32_to_string(array_indices[child_index], &key, str, 16);
    if (encoding == MONGO_STRING_UTF8) {
      bson_append_utf8(children[child_index], key, -1, value.c_str(), -1);
    }
    array_indices[child_index]++;
  }
  //! Add a string value to the open array in the buffer
  inline void add_string(std::string value) {add_string(value, MONGO_STRING_UTF8);}
  //! Add a string value to the buffer
  inline void add_string(std::string key, std::string value, int encoding) {
    if (encoding == MONGO_STRING_UTF8) {
      bson_append_utf8(children[child_index], key.c_str(), -1, value.c_str(), -1);
    }
  }
  inline void get_string(std::string key, std::string &value) {
    find_element(key);
    const char *val_cstr = bson_iter_utf8(&iterator, NULL);
    value.assign(val_cstr);
    //bson_free(val_cstr); // this may cause a memory leak, but want to try it
  }
  //! Add a boolean value to the buffer
  inline void add_bool(std::string key, bool value) {
    bson_append_bool(children[child_index], key.c_str(), -1, value);
  }
  //! Add a boolean value to the open array in the buffer
  inline void add_bool(bool value) {
    char str[16];
    const char *key;
    bson_uint32_to_string(array_indices[child_index], &key, str, 16);
    bson_append_bool(children[child_index], key, -1, value);
    array_indices[child_index]++;
  }
  inline bool get_bool(std::string key) {
    find_element(key);
    return bson_iter_bool(&iterator);
  }
  //! Add an integer value to the buffer
  inline void add_int(std::string key, int value) {
    bson_append_int32(children[child_index], key.c_str(), -1, value);
  }
  //! Add an integer value to the open array in the buffer
  inline void add_int(int value) {
    char str[16];
    const char *key;
    bson_uint32_to_string(array_indices[child_index], &key, str, 16);
    bson_append_int32(children[child_index], key, -1, value);
    array_indices[child_index]++;
  }
  inline int get_int(std::string key) {
    find_element(key);
    return bson_iter_int32(&iterator);
  }
  //! Add a double value to the buffer
  void add_double(std::string key, double value) {bson_append_double(children[child_index], key.c_str(), -1, value);}
  //! Add a double value to the open array in the buffer
  inline void add_double(double value) {
    char str[16];
    const char *key;
    bson_uint32_to_string(array_indices[child_index], &key, str, 16);
    bson_append_double(children[child_index], key, -1, value);
    array_indices[child_index]++;
  }
  inline double get_double(std::string key) {
    find_element(key);
    return bson_iter_double(&iterator);
  }
  //! Add a datetime value to the buffer
  void add_date(std::string key, int value) {
    bson_append_date_time(children[child_index], key.c_str(), -1, value);
  }
  //! Add an datetime value to the open array in the buffer
  void add_date(int value) {
    char str[16];
    const char *key;
    bson_uint32_to_string(array_indices[child_index], &key, str, 16);
    bson_append_date_time(children[child_index], key, -1, value);
    array_indices[child_index]++;
  }
  inline int get_date(std::string key) {
    find_element(key);
    return bson_iter_date_time(&iterator);
  }
  //! Start an array in the buffer
  inline void start_array(std::string key) {
    bson_t *child = new bson_t;
    children.push_back(child);
    array_indices.push_back(0);
    parent_index=child_index;
    child_index++;
    bson_append_array_begin(children[parent_index], key.c_str(), -1, children[child_index]);
  }
  //! End an array value in the buffer
  inline void end_array() {
    bson_append_array_end(children[parent_index], children[child_index]);
    if (parent_index > 0) {parent_index--;}
    if (child_index > 0) {child_index--;}
  }
  //! Start a subdocument in the buffer
  inline void start_object(std::string key) {
    bson_t *child = new bson_t;
    children.push_back(child);
    array_indices.push_back(0);
    parent_index=child_index;
    child_index++;
    bson_append_document_begin (children[parent_index], key.c_str(), -1, children[child_index]);
  }
  //! End a subdocument in the buffer
  inline void end_object() {
    bson_append_document_end(children[parent_index], children[child_index]);
    if (parent_index > 0) {parent_index--;}
    if (child_index > 0) {child_index--;}
  }
  //! Count the number of keys in the BSON Document
  int count_keys() {
    // int key_count = 0;
    // for (unsigned int i = 0; i < children.size(); i++) {
    //   key_count = key_count + bson_count_keys(children[i]);
    // }
    // return key_count;
    return bson_count_keys(children[0]);
  }
  //! Is a specific key present in the BSON Document
  bool has_field(std::string key) {
    const char *key_str = key.c_str();
    for (unsigned int i = 0; i < children.size(); i++) {
      if (bson_has_field(children[i], key_str)) {
        return true;
      }
    }
    return false;
  }
  //! Convert the BSON Buffer to a JSON Document and store it in json_string
  std::string to_json() {
    return to_json(256);
  }
  //! Convert the BSON Buffer to a JSON Document and store it in json_string
  std::string to_json(int string_length) {
    if (!json_cstring) {
      json_cstring = bson_as_json(children[0], NULL);
      if (!json_cstring) throw BsonException("JSON Translation failed for BSON Document");
      json_string.assign(json_cstring);
    }
    return json_string;
  }
};

}  // namespace AOSSL
#endif  // AOSSL_MONGO_INCLUDE_MONGO_BUFFER_H_
