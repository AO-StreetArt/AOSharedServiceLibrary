#include <log4cpp/Category.hh>
#include <string>

#ifndef LOGGING_INTERFACE
#define LOGGING_INTERFACE

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

  //! Pull down the root category
  virtual LoggingInterface get_root() = 0;
}

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

  //! Pull down different categories by name
  virtual LoggingCategoryInterface* get_category(std::string name) = 0;
};

extern LoggingInterface *logging;

#endif
