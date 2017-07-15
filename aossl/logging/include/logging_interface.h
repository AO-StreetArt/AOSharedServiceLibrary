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

#ifndef AOSSL_LOGGING_INCLUDE_LOGGING_INTERFACE_H_
#define AOSSL_LOGGING_INCLUDE_LOGGING_INTERFACE_H_

#include <log4cpp/Category.hh>
#include <string>
#include <exception>

const int AOSSL_LOG_DEBUG = 0;
const int AOSSL_LOG_INFO = 1;
const int AOSSL_LOG_ERROR = 2;

//! Logging Exception, used to store errors passed from the Logger
struct LoggingException: public std::exception {
  //! An error message passed on initialization
  std::string int_msg;
  const char * what_str;

  //! Create a Logging Exception, and store the given error message
  inline LoggingException(std::string msg) {
    int_msg = "Error in Logging Driver: " + msg;
    what_str = int_msg.c_str();
  }

  LoggingException() {}
  ~LoggingException() throw() {}

  //! Show the error message in readable format
  const char * what() const throw() {
    return what_str;
  }
};

//! A Logging Category instantiated on a standard logging instance
class LoggingCategoryInterface {
 public:
  virtual ~LoggingCategoryInterface() {}

  // Exposures that log to the sepcified category

  //! Log at a debug level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void debug(std::string msg) = 0;

  //! Log at an error level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void error(std::string msg) = 0;

  //! Log at an info level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void info(std::string msg) = 0;

  //! Log at an debug level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void debug(const char * msg) = 0;

  //! Log at an error level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void error(const char * msg) = 0;

  //! Log at an info level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void info(const char * msg) = 0;

  //! Log at an debug level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void debug(int msg) = 0;

  //! Log at an error level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void error(int msg) = 0;

  //! Log at an info level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void info(int msg) = 0;

  //! Log at an debug level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void debug(float msg) = 0;

  //! Log at an error level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void error(float msg) = 0;

  //! Log at an info level to the root category
  virtual void info(float msg) = 0;

  //! Log at an debug level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void debug(double msg) = 0;

  //! Log at an error level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void error(double msg) = 0;

  //! Log at an info level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void info(double msg) = 0;
};

//! An overall logging interface, which can generate logging categories
class LoggingInterface {
 public:
  virtual ~LoggingInterface() {}

  // Exposures that log to the root category

  //! Log at a debug level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void debug(std::string msg) = 0;

  //! Log at an error level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void error(std::string msg) = 0;

  //! Log at an info level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void info(std::string msg) = 0;

  //! Log at an debug level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void debug(const char * msg) = 0;

  //! Log at an error level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void error(const char * msg) = 0;

  //! Log at an info level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void info(const char * msg) = 0;

  //! Log at an debug level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void debug(int msg) = 0;

  //! Log at an error level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void error(int msg) = 0;

  //! Log at an info level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void info(int msg) = 0;

  //! Log at an debug level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void debug(float msg) = 0;

  //! Log at an error level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void error(float msg) = 0;

  //! Log at an info level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void info(float msg) = 0;

  //! Log at an debug level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void debug(double msg) = 0;

  //! Log at an error level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void error(double msg) = 0;

  //! Log at an info level to the root category
  /*!
    \param msg The message to print to the log appenders
  */
  virtual void info(double msg) = 0;

  //! Pull down different categories by name.  If we were configured from a
  //! Properties file, then we should pull the category names from there.
  //! Otherwise, this method will create a new category with the given name
  /*!
    \param name The name of the logging category
  */
  virtual LoggingCategoryInterface* get_category(std::string name) = 0;
};

//! The global logging pointer that is instantiated and deleted at
//! startup/shutdown, respectively
extern LoggingInterface *logging;

#endif  // AOSSL_LOGGING_INCLUDE_LOGGING_INTERFACE_H_
