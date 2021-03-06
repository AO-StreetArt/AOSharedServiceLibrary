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

#include <sys/stat.h>
#include <unordered_map>
#include <string>
#include <iterator>
#include <sstream>
#include <fstream>
#include <vector>

#include "aossl/core/include/kv_store.h"

#ifndef AOSSL_PROPERTIES_INCLUDE_PROPERTIES_READER_H_
#define AOSSL_PROPERTIES_INCLUDE_PROPERTIES_READER_H_

namespace AOSSL {

class PropertiesReader: public KeyValueStore {
  std::string config_file;

 public:
  // Create a new Command Line Interpreter
  // Here we create a new interpreter by passing in a single argument, the
  // address of a properties file.
  explicit PropertiesReader(std::string& file_path) : KeyValueStore() {
    std::string line;
    std::ifstream file(file_path.c_str());

    if (file.is_open()) {
      while (getline(file, line)) {
        // Read a line from the property file

        // Figure out if we have a blank or comment line
        if (line.length() > 0) {
          if (!(line[0] == '/' && line[1] == '/')) {
            std::size_t eq_pos = line.find("=", 0);
            if (eq_pos != std::string::npos) {
              std::string var_name = line.substr(0, eq_pos);
              std::string var_value = \
                line.substr(eq_pos+1, line.length() - eq_pos);
              KeyValueStore::set_opt(var_name, var_value);
            }
          }
        }
      }
      file.close();
    }
  }

  ~PropertiesReader() {}

  void load_config() {}
};

}  // namespace AOSSL

#endif  // AOSSL_PROPERTIES_INCLUDE_PROPERTIES_READER_H_
