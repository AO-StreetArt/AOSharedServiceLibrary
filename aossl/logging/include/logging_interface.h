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

#ifndef LOGGING_INTERFACE
#define LOGGING_INTERFACE

#include <log4cpp/Category.hh>
#include <string>
#include <exception>

//! Mongo Exception, used to store errors passed from Mongo
struct LoggingException: public std::exception
{
  //! An error message passed on initialization
  std::string int_msg;
  const char * what_str;

  //! Create a Mongo Exception, and store the given error message
  LoggingException (std::string msg) {int_msg = "Error in Logging Driver: " + msg;what_str = int_msg.c_str();}

  LoggingException () {}
  ~LoggingException() throw () {}

  //! Show the error message in readable format
  const char * what() const throw ()
  {
    return what_str;
  }
};

//! A Logging Category instantiated on a standard logging instance
class LoggingCategoryInterface
{
public:
  virtual ~LoggingCategoryInterface() {}

  //Exposures that log to the sepcified category

  //! Log at a debug level to the root category
  virtual void debug(std::string msg) = 0;

  //! Log at an error level to the root category
  virtual void error(std::string msg) = 0;

  //! Log at an info level to the root category
  virtual void info(std::string msg) = 0;

  //! Log at an debug level to the root category
  virtual void debug(const char * msg) = 0;

  //! Log at an error level to the root category
  virtual void error(const char * msg) = 0;

  //! Log at an info level to the root category
  virtual void info(const char * msg) = 0;

  //! Log at an debug level to the root category
  virtual void debug(int msg) = 0;

  //! Log at an error level to the root category
  virtual void error(int msg) = 0;

  //! Log at an info level to the root category
  virtual void info(int msg) = 0;

  //! Log at an debug level to the root category
  virtual void debug(float msg) = 0;

  //! Log at an error level to the root category
  virtual void error(float msg) = 0;

  //! Log at an info level to the root category
  virtual void info(float msg) = 0;

  //! Log at an debug level to the root category
  virtual void debug(double msg) = 0;

  //! Log at an error level to the root category
  virtual void error(double msg) = 0;

  //! Log at an info level to the root category
  virtual void info(double msg) = 0;
};

//! An overall logging interface, which can generate logging categories
class LoggingInterface
{
public:

  virtual ~LoggingInterface() {}

  //Exposures that log to the root category

  //! Log at a debug level to the root category
  virtual void debug(std::string msg) = 0;

  //! Log at an error level to the root category
  virtual void error(std::string msg) = 0;

  //! Log at an info level to the root category
  virtual void info(std::string msg) = 0;

  //! Log at an debug level to the root category
  virtual void debug(const char * msg) = 0;

  //! Log at an error level to the root category
  virtual void error(const char * msg) = 0;

  //! Log at an info level to the root category
  virtual void info(const char * msg) = 0;

  //! Log at an debug level to the root category
  virtual void debug(int msg) = 0;

  //! Log at an error level to the root category
  virtual void error(int msg) = 0;

  //! Log at an info level to the root category
  virtual void info(int msg) = 0;

  //! Log at an debug level to the root category
  virtual void debug(float msg) = 0;

  //! Log at an error level to the root category
  virtual void error(float msg) = 0;

  //! Log at an info level to the root category
  virtual void info(float msg) = 0;

  //! Log at an debug level to the root category
  virtual void debug(double msg) = 0;

  //! Log at an error level to the root category
  virtual void error(double msg) = 0;

  //! Log at an info level to the root category
  virtual void info(double msg) = 0;

  //! Pull down different categories by name
  virtual LoggingCategoryInterface* get_category(std::string name) = 0;
};

//! The global logging pointer that is instantiated and deleted at startup/shutdown, respectively
extern LoggingInterface *logging;

#endif
