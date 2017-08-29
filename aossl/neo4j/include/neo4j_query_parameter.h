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

#ifndef AOSSL_NEO4J_INCLUDE_NEO4J_QUERY_PARAMETER_H_
#define AOSSL_NEO4J_INCLUDE_NEO4J_QUERY_PARAMETER_H_

#include <cstdlib>
#include <stdio.h>
#include <errno.h>
#include <cstring>
#include <neo4j-client.h>
#include <string>
#include <exception>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <mutex>
#include "neo4j_interface.h"
#include "neo4j_utils.h"


// A Query parameter to be inserted into cypher queries
// Can be either a single value, or a list of values
class Neo4jQueryParameter: public Neo4jQueryParameterInterface {
  // Internal Variables
  bool bool_value;
  std::string str_value;
  const char * cstr_value;
  int int_value;
  double double_value;
  int type;
  bool is_list;
  std::vector<bool> bool_values;
  std::vector<std::string> str_values;
  std::vector<const char*> cstr_values;
  std::vector<int> int_values;
  std::vector<double> double_values;
  neo4j_value_t list_val;
  // Internal Variable for storing an array of pointers to neo4j_values
  neo4j_value_t list_values[30];
  // Input True if we are performing a list operation
  // False if not
  // Throw an exception if we are performing an invalid operation
  inline void list_check(bool list_op) {
    if (((!is_list) && list_op) || (is_list && !(list_op))) {
      std::string err_msg = "Invalid List Operation";
      throw Neo4jException(err_msg);
    }
  }
  // Input the type of list operation being performed
  // Throw an exception if we are performing an invalid operation
  inline void type_check(int inp_type) {
    if (type != -1 && type != inp_type) {
      std::string err_msg = "Invalid Type Operation";
      throw Neo4jException(err_msg);
    }
  }

 public:
  // Constructors
  Neo4jQueryParameter() {is_list = true; type = -1;}
  inline Neo4jQueryParameter(bool inp_bool) {
    bool_value = inp_bool;
    type = _BOOL_TYPE;
    is_list = false;
  }
  inline Neo4jQueryParameter(std::string inp_str) {
    str_value = inp_str;
    type = _STR_TYPE;
    is_list = false;
    cstr_value = str_value.c_str();
  }
  inline Neo4jQueryParameter(const char * inp_str) {
    cstr_value = inp_str;
    std::string new_val(cstr_value);
    str_value = new_val;
    type = _STR_TYPE;
    is_list = false;
  }
  inline Neo4jQueryParameter(int inp_int) {
    int_value = inp_int;
    type = _INT_TYPE;
    is_list = false;
  }
  inline Neo4jQueryParameter(double inp_double) {
    double_value = inp_double;
    type = _FLT_TYPE;
    is_list = false;
  }

  // Destructor
  inline ~Neo4jQueryParameter() {
    bool_values.clear();
    str_values.clear();
    int_values.clear();
    double_values.clear();
    for (int i = 0; i < cstr_values.size(); i++) {
      delete[] cstr_values[i];
    }
  }

  // What type of query parameter is this?
  int get_type() {return type;}

  // Retrieve a boolean value from a single parameter
  inline bool get_boolean_value() {
    list_check(false);
    type_check(_BOOL_TYPE);
    return bool_value;
  }

  // Retrieve a boolean value from a list parameter
  inline bool get_boolean_value(int index) {
    list_check(true);
    type_check(_BOOL_TYPE);
    return bool_values[index];
  }

  // Retrieve a string value from a single parameter
  inline std::string get_string_value() {
    list_check(false);
    type_check(_STR_TYPE);
    return str_value;
  }

  // Retrieve a string value from a list parameter
  inline std::string get_string_value(int index) {
    list_check(true);
    type_check(_STR_TYPE);
    return str_values[index];
  }

  // Retrieve a string value from a single parameter
  inline const char * get_cstring_value() {
    list_check(false);
    type_check(_STR_TYPE);
    return cstr_value;
  }

  inline const char * get_cstring_value(int index) {
    list_check(false);
    type_check(_STR_TYPE);
    return cstr_values[index];
  }

  // Retrieve an integer value from a single parameter
  inline int get_integer_value() {
    list_check(false);
    type_check(_INT_TYPE);
    return int_value;
  }

  // Retrieve an integer value from a list parameter
  inline int get_integer_value(int index) {
    list_check(true);
    type_check(_INT_TYPE);
    return int_values[index];
  }

  // Retrieve a double value from a single parameter
  inline double get_double_value() {
    list_check(false);
    type_check(_FLT_TYPE);
    return double_value;
  }

  // Retrieve a double value from a list parameter
  inline double get_double_value(int index) {
    list_check(true);
    type_check(_FLT_TYPE);
    return double_values[index];
  }

  // Is the query  parameter a list?
  bool is_array() {return is_list;}

  // What is the length of the list query parameter?
  inline unsigned int size() {
    list_check(true);
    if (type == _FLT_TYPE) {
      return double_values.size();
    } else if (type == _INT_TYPE) {
      return int_values.size();
    } else if (type == _STR_TYPE) {
      return str_values.size();
    } else if (type == _BOOL_TYPE) {
      return bool_values.size();
    } else {
      return 0;
    }
  }

  // Add a boolean value to the list parameter
  inline void add_value(bool new_val) {
    list_check(true);
    type_check(_BOOL_TYPE);
    bool_values.push_back(new_val);
    type = _BOOL_TYPE;
  }

  // Add a string value to the list parameter
  inline void add_value(std::string new_val) {
    list_check(true);
    type_check(_STR_TYPE);
    str_values.push_back(new_val);
    type = _STR_TYPE;
  }

  // Add a string value to the list parameter
  inline void add_value(const char * new_val) {
    std::string new_str(new_val);
    add_value(new_str);
  }

  // Add an integer value to the list parameter
  inline void add_value(int new_val) {
    list_check(true);
    type_check(_INT_TYPE);
    int_values.push_back(new_val);
    type = _INT_TYPE;
  }

  // Add a float value to the list parameter
  inline void add_value(float new_val) {
    list_check(true);
    type_check(_FLT_TYPE);
    double_values.push_back(new_val);
    type = _FLT_TYPE;
  }

  inline neo4j_value_t get_neo4j_list() {
    int val_type = get_type();
    // If we have a string type parameter, copy the values over to the C String
    // Vector prior to starting
    if (val_type == _STR_TYPE) {
      cstr_values.reserve(size());
      for (int i = 0; i < size(); i++) {
        int cstr_size = get_string_value(i).size();
        char * cstr_val = new char[cstr_size];
        std::strcpy(cstr_val, get_string_value(i).c_str());
        cstr_values.push_back(cstr_val);
      }
    }
    // Build an array of neo4j values to hold the list elements
    // Iterate over the values in the list parameter, filling up our array
    for (unsigned int j = 0; j < size(); j++) {
      if (val_type == _BOOL_TYPE) {
        list_values[j] = neo4j_bool(get_boolean_value(j));
      } else if (val_type == _STR_TYPE) {
        if (get_string_value(j).empty()) {
          throw Neo4jException("query parameters must have length > 1.");
        }
        list_values[j] = neo4j_string(cstr_values[j]);
      } else if (val_type == _INT_TYPE) {
        list_values[j] = neo4j_int(get_integer_value(j));
      } else if (val_type == _FLT_TYPE) {
        list_values[j] = neo4j_float(get_double_value(j));
      }
    }
    list_val = neo4j_list(list_values, size());
    return list_val;
  }
};

#endif  // AOSSL_NEO4J_INCLUDE_NEO4J_QUERY_PARAMETER_H_
