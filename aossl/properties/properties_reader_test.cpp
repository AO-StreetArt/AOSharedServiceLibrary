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
#include <vector>
#include <string>
#include "include/factory_props.h"
#include "include/properties_reader_interface.h"

int main(int argc, char** argv) {
  AOSSL::PropertyReaderFactory prop_factory;
  AOSSL::PropertiesReaderInterface *reader = \
    prop_factory.get_properties_reader_interface("test.properties");
  if ( reader->opt_exist("DB_Password") ) {
    assert(reader->get_opt("DB_Password") == "default");
  } else {
    assert(false);
  }

  if ( reader->list_exist("RedisConnectionString") ) {
    std::vector<std::string> list = reader->get_list("RedisConnectionString");
    assert(list.size() == 2);
  } else {
    assert(false);
  }

  delete reader;
  return 0;
}
