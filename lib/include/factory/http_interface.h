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

//The HTTP Administrator
//Responsible for placing HTTP Requests using curl

#include <string>

#ifndef HTTP_INTERFACE
#define HTTP_INTERFACE

//! An Implementation of std::exception that denotes an error during an http operation
struct HttpRequestException: public std::exception
{
  //! An error message passed on initialization
  std::string int_msg;

  //! Create a HTTP Request Exception, and store the given error message
  HttpRequestException (std::string msg) {int_msg = msg;}

  HttpRequestException () {}
  ~HttpRequestException() throw () {}

  //! Show the error message in readable format
  const char * what() const throw ()
  {
    std::string what_str = "Error Sending HTTP Request: " + int_msg;
    return what_str.c_str();
  }
};

typedef size_t (*WriteCallback)(char *, size_t, size_t, void*);

//! The HTTP Requests Administrators

//! This class is in charge of making HTTP Requests
//! Support for put, get post, and delete
class HttpInterface
{
public:

  //! Shutdown the admin
  virtual void shutdown() = 0;

  virtual ~HttpInterface() {}

  //HTTP Methods

  //! Put

  //! Put to the given URL the supplied data
  //! with the specified timeout
  virtual bool put(std::string url, std::string data, int timeout) = 0;

  //! Get

  //! Get from the given URL
  //! with the specified timeout
  virtual std::string get(std::string url, int timeout) = 0;

  //! Post

  //! Post to the given URL the supplied data
  //! with the specified timeout
  virtual bool post(std::string url, std::string data, int timeout) = 0;

  //! Delete

  //! Delete from the given URL
  //! with the specified timeout
  virtual bool del(std::string url, int timeout) = 0;
};

#endif
