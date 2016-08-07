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
std::unordered_map<std::string, log4cpp::Category *log> logs;
log4cpp::Category *root_log;
void start_log_from_file(std::string initFileName);
void end_log();
public:

  //Constructor & Destructor
  Logger(std::string initFileName);
  ~Logger();

  //Exposures that log to the root category
  void debug(std::string msg);
  void error(std::string msg);
  void info(std::string msg);
  void debug(const char * msg);
  void error(const char * msg);
  void info(const char * msg);

  //Pull down different categories by name
  log4cpp::Category* get_category(std::string name);

  //Pull down the root category directly
  log4cpp::Category* get_root();
}

extern Logger *logging;

#endif
