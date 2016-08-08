#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <string>
#include <stdlib.h>
#include <unordered_map>
#include <vector>

#ifndef LYMAN_LOG
#define LYMAN_LOG

class Logger
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

  //! Pull down different categories by name
  log4cpp::Category& get_category(std::string name);

  //! Pull down the root category directly
  log4cpp::Category* get_root();
};

extern Logger *logging;

#endif
