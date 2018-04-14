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

#ifndef AOSSL_MONGO_INCLUDE_MONGO_BUFFER_INTERFACE_H_
#define AOSSL_MONGO_INCLUDE_MONGO_BUFFER_INTERFACE_H_

#include <bson.h>
#include <mongoc.h>
#include <exception>
#include <string>
#include <unordered_map>

#include "aossl/core/include/buffers.h"

namespace AOSSL {

const int MONGO_STRING_UTF8 = 0;

//! BSON Exception, used to store document-related errors
struct BsonException: public std::exception {
  //! An error message passed on initialization
  std::string int_msg;
  const char * what_str;

  //! Create a Mongo Exception, and store the given error message
  inline BsonException(std::string msg) {
    int_msg = "Error in Bson Document: " + msg;
    what_str = int_msg.c_str();
  }

  BsonException() {}
  ~BsonException() throw() {}

  //! Show the error message in readable format
  const char * what() const throw() {
    return what_str;
  }
};

//! A Structure for holding a BSON Document
class MongoBufferInterface {
 public:
  virtual ~MongoBufferInterface() {}
  //! Is a successful response
  virtual bool successful() = 0;
  //! Error message
  virtual std::string get_error_message() = 0;
  //! Get the underlying bson_t document to pass to the mongoc driver
  virtual bson_t* get_bson() = 0;
  //! Add an OID value to the buffer
  virtual void add_oid(std::string value) = 0;
  //! Add a string value to the buffer
  virtual void add_string(std::string key, std::string value) = 0;
  //! Add a string value to the open array in the buffer
  virtual void add_string(std::string value, int encoding) = 0;
  //! Add a string value to the open array in the buffer
  virtual void add_string(std::string value) = 0;
  //! Add a string value to the buffer
  virtual void add_string(std::string key, std::string value, int encoding) = 0;
  //! Add a boolean value to the buffer
  virtual void add_bool(std::string key, bool value) = 0;
  //! Add a boolean value to the open array in the buffer
  virtual void add_bool(bool value) = 0;
  //! Add an integer value to the buffer
  virtual void add_int(std::string key, int value) = 0;
  //! Add an integer value to the open array in the buffer
  virtual void add_int(int value) = 0;
  //! Add a double value to the buffer
  virtual void add_double(std::string key, double value) = 0;
  //! Add a double value to the open array in the buffer
  virtual void add_double(double value) = 0;
  //! Add a datetime value to the buffer
  virtual void add_date(std::string key, int value) = 0;
  //! Add an datetime value to the open array in the buffer
  virtual void add_date(int value) = 0;
  //! Start an array in the buffer
  virtual void start_array(std::string key) = 0;
  //! End an array value in the buffer
  virtual void end_array() = 0;
  //! Start a subdocument in the buffer
  virtual void start_object(std::string key) = 0;
  //! End a subdocument in the buffer
  virtual void end_object() = 0;
  //! Count the number of keys in the BSON Document
  virtual int count_keys() = 0;
  //! Is a specific key present in the BSON Document
  virtual bool has_field(std::string key) = 0;
  //! Convert the BSON Buffer to a JSON Document and store internally in the Mongo Buffer

  //! Note that the converted message will be retained in memory until the
  //! Mongo Buffer is destroyed.  Not thread-safe
  virtual std::string to_json() = 0;
  //! Convert the BSON Buffer to a JSON Document and store internally in the Mongo Buffer

  //! Note that the converted message will be retained in memory until the
  //! Mongo Buffer is destroyed.  Not thread-safe
  virtual std::string to_json(int string_length) = 0;
};

}  // namespace AOSSL
#endif  // AOSSL_MONGO_INCLUDE_MONGO_BUFFER_INTERFACE_H_
