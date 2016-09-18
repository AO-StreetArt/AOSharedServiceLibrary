#include "include/logging.h"

//Start the logger from a properties file
void Logger::start_log_from_file(std::string initFileName)
{
  //Read the properties file
  try {
    log4cpp::PropertyConfigurator::configure(initFileName);
  }
  catch ( log4cpp::ConfigureFailure &e ) {
    std::cout << "[log4cpp::ConfigureFailure] caught while reading" << initFileName << std::endl;
    std::cout << e.what();
    exit(1);
  }

  //Find the categories defined in the file
  // std::vector<log4cpp::Category*> categories = log4cpp::Category::getCurrentCategories();

  //Expose the logging handles
  log4cpp::Category& root = log4cpp::Category::getRoot();
  // log4cpp::Category& sub1 = log4cpp::Category::getInstance(std::string("sub1"));

  root_log = &root;
}

//Bring down the logging module
void Logger::end_log()
{
  if (!root_log) {
    std::cout << "Shutdown called with no logging module present" << std::endl;
  }
  else {
    log4cpp::Category::shutdown();
  }
}

//Constructor from a file
Logger::Logger(std::string initFileName)
{
start_log_from_file(initFileName);
}

//Destructor
Logger::~Logger()
{
  end_log();
}

//Exposures that log to the root category
void Logger::debug(std::string msg)
{
  if (!root_log)
  {
    std::cout << msg << std::endl;
  }
  else
  {
    root_log->debug(msg);
  }
}

void Logger::error(std::string msg)
{
  if (!root_log)
  {
    std::cout << msg << std::endl;
  }
  else
  {
    root_log->error(msg);
  }
}

void Logger::info(std::string msg)
{
  if (!root_log)
  {
    std::cout << msg << std::endl;
  }
  else
  {
    root_log->info(msg);
  }
}

void Logger::debug(const char * msg)
{
  if (!root_log)
  {
    std::cout << msg << std::endl;
  }
  else
  {
    root_log->debug(msg);
  }
}

void Logger::error(const char * msg)
{
  if (!root_log)
  {
    std::cout << msg << std::endl;
  }
  else
  {
    root_log->error(msg);
  }
}

void Logger::info(const char * msg)
{
  if (!root_log)
  {
    std::cout << msg << std::endl;
  }
  else
  {
    root_log->info(msg);
  }
}

void Logger::debug(int msg)
{
  if (!root_log)
  {
    std::cout << msg << std::endl;
  }
  else
  {
    root_log->debug(std::to_string(msg));
  }
}

void Logger::error(int msg)
{
  if (!root_log)
  {
    std::cout << msg << std::endl;
  }
  else
  {
    root_log->error(std::to_string(msg));
  }
}

void Logger::info(int msg)
{
  if (!root_log)
  {
    std::cout << msg << std::endl;
  }
  else
  {
    root_log->info(std::to_string(msg));
  }
}

void Logger::debug(float msg)
{
  if (!root_log)
  {
    std::cout << msg << std::endl;
  }
  else
  {
    root_log->debug(std::to_string(msg));
  }
}

void Logger::error(float msg)
{
  if (!root_log)
  {
    std::cout << msg << std::endl;
  }
  else
  {
    root_log->error(std::to_string(msg));
  }
}

void Logger::info(float msg)
{
  if (!root_log)
  {
    std::cout << msg << std::endl;
  }
  else
  {
    root_log->info(std::to_string(msg));
  }
}

void Logger::debug(double msg)
{
  if (!root_log)
  {
    std::cout << msg << std::endl;
  }
  else
  {
    root_log->debug(std::to_string(msg));
  }
}

void Logger::error(double msg)
{
  if (!root_log)
  {
    std::cout << msg << std::endl;
  }
  else
  {
    root_log->error(std::to_string(msg));
  }
}

void Logger::info(double msg)
{
  if (!root_log)
  {
    std::cout << msg << std::endl;
  }
  else
  {
    root_log->info(std::to_string(msg));
  }
}

//Pull down different categories by name
LoggingCategoryInterface* Logger::get_category(std::string name)
{
  return new LoggingCategory(name);
}

//--------------------------Logging Category Methods---------------------------//

//Exposures that log to the root category
void LoggingCategory::debug(std::string msg)
{
  if (!int_category)
  {
    std::cout << name << ": " << msg << std::endl;
  }
  else
  {
    int_category->debug(name + "::" + msg);
  }
}

void LoggingCategory::error(std::string msg)
{
  if (!int_category)
  {
    std::cout << name << ": " << msg << std::endl;
  }
  else
  {
    int_category->error(name + "::" + msg);
  }
}

void LoggingCategory::info(std::string msg)
{
  if (!int_category)
  {
    std::cout << name << ": " << msg << std::endl;
  }
  else
  {
    int_category->info(name + "::" + msg);
  }
}

void LoggingCategory::debug(const char * msg)
{
  if (!int_category)
  {
    std::cout << name << ": " << msg << std::endl;
  }
  else
  {
    int_category->debug(name + "::" + msg);
  }
}

void LoggingCategory::error(const char * msg)
{
  if (!int_category)
  {
    std::cout << name << ": " << msg << std::endl;
  }
  else
  {
    int_category->error(name + "::" + msg);
  }
}

void LoggingCategory::info(const char * msg)
{
  if (!int_category)
  {
    std::cout << name << ": " << msg << std::endl;
  }
  else
  {
    int_category->info(name + "::" + msg);
  }
}

void LoggingCategory::debug(int msg)
{
  if (!int_category)
  {
    std::cout << name << ": " << msg << std::endl;
  }
  else
  {
    int_category->debug(name + "::" + std::to_string(msg));
  }
}

void LoggingCategory::error(int msg)
{
  if (!int_category)
  {
    std::cout << name << ": " << msg << std::endl;
  }
  else
  {
    int_category->error(name + "::" + std::to_string(msg));
  }
}

void LoggingCategory::info(int msg)
{
  if (!int_category)
  {
    std::cout << name << ": " << msg << std::endl;
  }
  else
  {
    int_category->info(name + "::" + std::to_string(msg));
  }
}

void LoggingCategory::debug(float msg)
{
  if (!int_category)
  {
    std::cout << name << ": " << msg << std::endl;
  }
  else
  {
    int_category->debug(name + "::" + std::to_string(msg));
  }
}

void LoggingCategory::error(float msg)
{
  if (!int_category)
  {
    std::cout << name << ": " << msg << std::endl;
  }
  else
  {
    int_category->error(name + "::" + std::to_string(msg));
  }
}

void LoggingCategory::info(float msg)
{
  if (!int_category)
  {
    std::cout << name << ": " << msg << std::endl;
  }
  else
  {
    int_category->info(name + "::" + std::to_string(msg));
  }
}

void LoggingCategory::debug(double msg)
{
  if (!int_category)
  {
    std::cout << name << ": " << msg << std::endl;
  }
  else
  {
    int_category->debug(name + "::" + std::to_string(msg));
  }
}

void LoggingCategory::error(double msg)
{
  if (!int_category)
  {
    std::cout << name << ": " << msg << std::endl;
  }
  else
  {
    int_category->error(name + "::" + std::to_string(msg));
  }
}

void LoggingCategory::info(double msg)
{
  if (!int_category)
  {
    std::cout << name << ": " << msg << std::endl;
  }
  else
  {
    int_category->info(name + "::" + std::to_string(msg));
  }
}
