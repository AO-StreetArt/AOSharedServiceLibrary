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

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <string>
#include <stdlib.h>
#include <unordered_map>
#include <vector>

#include "factory/logging_interface.h"

#ifndef AOSSL_LOG
#define AOSSL_LOG

class LoggingCategory: public LoggingCategoryInterface
{
std::string name;
log4cpp::Category *int_category;
void start_log_from_file(std::string initFileName);
void end_log();
public:

  //! Build a new Logger from the given configuration file
  LoggingCategory(std::string iname) {name=iname;int_category = log4cpp::Category::exists(name);}

  //! Delete the Logger
  ~LoggingCategory() {}

  //Exposures that log to the chosen category

  //! Log at a debug level to the root category
  void debug(std::string msg);

  //! Log at an error level to the root category
  void error(std::string msg);

  //! Log at an info level to the root category
  void info(std::string msg);

  //! Log at an debug level to the root category
  void debug(const char * msg);

  //! Log at an error level to the root category
  void error(const char * msg);

  //! Log at an info level to the root category
  void info(const char * msg);

  //! Log at an debug level to the root category
  void debug(int msg);

  //! Log at an error level to the root category
  void error(int msg);

  //! Log at an info level to the root category
  void info(int msg);

  //! Log at an debug level to the root category
  void debug(float msg);

  //! Log at an error level to the root category
  void error(float msg);

  //! Log at an info level to the root category
  void info(float msg);

  //! Log at an debug level to the root category
  void debug(double msg);

  //! Log at an error level to the root category
  void error(double msg);

  //! Log at an info level to the root category
  void info(double msg);
};

class Logger: public LoggingInterface
{
log4cpp::Category *root_log;
void start_log_from_file(std::string initFileName);
void end_log();
public:

  //! Build a new Logger from the given configuration file
  Logger(std::string initFileName);

  //! Delete the Logger
  ~Logger();

  //Exposures that log to the root category

  //! Log at a debug level to the root category
  void debug(std::string msg);

  //! Log at an error level to the root category
  void error(std::string msg);

  //! Log at an info level to the root category
  void info(std::string msg);

  //! Log at an debug level to the root category
  void debug(const char * msg);

  //! Log at an error level to the root category
  void error(const char * msg);

  //! Log at an info level to the root category
  void info(const char * msg);

  //! Log at an debug level to the root category
  void debug(int msg);

  //! Log at an error level to the root category
  void error(int msg);

  //! Log at an info level to the root category
  void info(int msg);

  //! Log at an debug level to the root category
  void debug(float msg);

  //! Log at an error level to the root category
  void error(float msg);

  //! Log at an info level to the root category
  void info(float msg);

  //! Log at an debug level to the root category
  void debug(double msg);

  //! Log at an error level to the root category
  void error(double msg);

  //! Log at an info level to the root category
  void info(double msg);

  //! Pull down different categories by name
  LoggingCategoryInterface* get_category(std::string name);
};

#endif
