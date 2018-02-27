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
 public:
  //! Build a new Mongo Buffer
  MongoBuffer() {bson_t *b = new bson_t; bson_init (b); children.push_back(b);}
  //! Destroy the Mongo Buffer
  ~MongoBuffer() {for (int i = 0; i < children.size(); i++) {bson_destroy (children[i]); delete children[i];}}
  //! Is a successful response
  bool successful() {return Buffer::success;}
  //! Error message
  std::string get_error_message() {return Buffer::err_msg;}
  //! Get the underlying bson_t document to pass to the mongoc driver
  bson_t* get_bson() {return children[0];}
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
};

}  // namespace AOSSL
#endif  // AOSSL_MONGO_INCLUDE_MONGO_BUFFER_H_
