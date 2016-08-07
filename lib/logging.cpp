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
  std::vector<log4cpp::Category*> categories = log4cpp::Category::getCurrentCategories();

  for (int i =0; i < categories.size(); i++)
  {
    logs.emplace( categories[i]->getName(), categories[i] );
  }

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
  end_log()
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

//Pull down different categories by name
log4cpp::Category* Logger::get_category(std::string name)
{
  auto search = logs.find(name);
  if (search != logs.end())
  {
    return logs[name];
  }
  else
  {
    if (!root_log) {
      std::cout << "Bad Logger Configuration, no root or category found" << std::endl;
    }
    else {
      return root_log;
      info("Category not found: ");
      info(name);
    }
  }
}

//Pull down different categories by name
log4cpp::Category* Logger::get_root()
{
  return root_log;
}

Logger *logging;
