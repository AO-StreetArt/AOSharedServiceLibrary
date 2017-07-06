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

#include "include/properties_reader.h"

// Constructor
PropertiesReader::PropertiesReader(std::string file_path) {
  // Open the file
  std::string line;

  // Check if the file exists
  struct stat buffer;
  if (stat(file_path.c_str(), &buffer) == 0) {
    std::ifstream file(file_path);

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
              opts.emplace(std::make_pair(var_name, var_value));
            } else {
              // We have a line that's non-blank and not a comment, as well
              // as not a key-value pair.  In this case, we assume that we
              // have a list, which uses the syntax -list_name-list_value
              std::size_t eq_pos = line.find("-", 1);
              if (eq_pos != std::string::npos) {
                std::string list_name = line.substr(1, eq_pos - 1);
                std::string list_value = \
                  line.substr(eq_pos+1, line.length() - eq_pos);
                if (!list_exist(list_name)) {
                  // Create a new list and add it to the map
                  std::vector<std::string> val_list;
                  val_list.push_back(list_value);
                  opt_lists.emplace(std::make_pair(list_name, val_list));
                } else {
                  // Add to an existing list in the map
                  opt_lists[list_name].push_back(list_value);
                }
              }
            }
          }
        }
      }
      file.close();
    }
  }
}

// Does a key exist?
bool PropertiesReader::opt_exist(std::string key) {
  auto search = opts.find(key);
  if (search != opts.end()) return true;
  return false;
}

// Does a key exist?
bool PropertiesReader::list_exist(std::string key) {
  auto search = opt_lists.find(key);
  if (search != opt_lists.end()) return true;
  return false;
}
