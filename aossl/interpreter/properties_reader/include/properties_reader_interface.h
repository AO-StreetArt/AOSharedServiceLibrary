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

#include <string>
#include <vector>

#ifndef PROPERTIES_READER_INTERFACE
#define PROPERTIES_READER_INTERFACE

//! PropertiesReaderInterface

//! Here we create a new interpreter by passing in a single argument, the address
//! of a properties file.  This file is opened and read, with properties in the form:
//! property_name=property_value
//! This also accepts lists in the form
//! -list_name-list_value
//! -list_name-list_value2
class PropertiesReaderInterface
{
public:

  virtual ~PropertiesReaderInterface() {}

  //! Does a key exist?
  virtual bool opt_exist( std::string key ) = 0;

  //! Get an option by key
  virtual std::string get_opt( std::string key ) = 0;

  //! Does a list exist within the configuration?
  virtual bool list_exist( std::string key ) = 0;

  //! Get a list that exists within the configuration
  virtual std::vector<std::string> get_list( std::string key ) = 0;

};

#endif
