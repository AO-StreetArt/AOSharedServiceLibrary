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

#include "include/logging.h"

// Start the logger from a properties file
void Logger::start_log_from_file(std::string initFileName) {
  // Read the properties file
  try {
    log4cpp::PropertyConfigurator::configure(initFileName);
  }
  catch ( log4cpp::ConfigureFailure &e ) {
    std::string err_msg = "[log4cpp::ConfigureFailure] caught while reading" +\
      initFileName;
    throw LoggingException(err_msg);
  }

  // Expose the logging handles
  log4cpp::Category& root = log4cpp::Category::getRoot();

  root_log = &root;
}

// Bring down the logging module
void Logger::end_log() {
  if (!root_log) {
    std::cout << "Shutdown called with no logging module present" << std::endl;
  } else {
    log4cpp::Category::shutdown();
  }
}

// Constructor from a file
Logger::Logger(std::string initFileName) {
  start_log_from_file(initFileName);
}

// Destructor
Logger::~Logger() {
  end_log();
}

// Exposures that log to the root category
void Logger::debug(std::string msg) {
  if (!root_log) {
    std::cout << msg << std::endl;
  } else {
    root_log->debug(msg);
  }
}

void Logger::error(std::string msg) {
  if (!root_log) {
    std::cout << msg << std::endl;
  } else {
    root_log->error(msg);
  }
}

void Logger::info(std::string msg) {
  if (!root_log) {
    std::cout << msg << std::endl;
  } else {
    root_log->info(msg);
  }
}

void Logger::debug(const char * msg) {
  if (!root_log) {
    std::cout << msg << std::endl;
  } else {
    root_log->debug(msg);
  }
}

void Logger::error(const char * msg) {
  if (!root_log) {
    std::cout << msg << std::endl;
  } else {
    root_log->error(msg);
  }
}

void Logger::info(const char * msg) {
  if (!root_log) {
    std::cout << msg << std::endl;
  } else {
    root_log->info(msg);
  }
}

void Logger::debug(int msg) {
  if (!root_log) {
    std::cout << msg << std::endl;
  } else {
    root_log->debug(std::to_string(msg));
  }
}

void Logger::error(int msg) {
  if (!root_log) {
    std::cout << msg << std::endl;
  } else {
    root_log->error(std::to_string(msg));
  }
}

void Logger::info(int msg) {
  if (!root_log) {
    std::cout << msg << std::endl;
  } else {
    root_log->info(std::to_string(msg));
  }
}

void Logger::debug(float msg) {
  if (!root_log) {
    std::cout << msg << std::endl;
  } else {
    root_log->debug(std::to_string(msg));
  }
}

void Logger::error(float msg) {
  if (!root_log) {
    std::cout << msg << std::endl;
  } else {
    root_log->error(std::to_string(msg));
  }
}

void Logger::info(float msg) {
  if (!root_log) {
    std::cout << msg << std::endl;
  } else {
    root_log->info(std::to_string(msg));
  }
}

void Logger::debug(double msg) {
  if (!root_log) {
    std::cout << msg << std::endl;
  } else {
    root_log->debug(std::to_string(msg));
  }
}

void Logger::error(double msg) {
  if (!root_log) {
    std::cout << msg << std::endl;
  } else {
    root_log->error(std::to_string(msg));
  }
}

void Logger::info(double msg) {
  if (!root_log) {
    std::cout << msg << std::endl;
  } else {
    root_log->info(std::to_string(msg));
  }
}

// Pull down different categories by name
LoggingCategoryInterface* Logger::get_category(std::string name) {
  return new LoggingCategory(name);
}

// Logging Category Methods

// Exposures that log to the root category
void LoggingCategory::debug(std::string msg) {
  if (!int_category) {
    std::cout << name << ": " << msg << std::endl;
  } else {
    int_category->debug(name + "::" + msg);
  }
}

void LoggingCategory::error(std::string msg) {
  if (!int_category) {
    std::cout << name << ": " << msg << std::endl;
  } else {
    int_category->error(name + "::" + msg);
  }
}

void LoggingCategory::info(std::string msg) {
  if (!int_category) {
    std::cout << name << ": " << msg << std::endl;
  } else {
    int_category->info(name + "::" + msg);
  }
}

void LoggingCategory::debug(const char * msg) {
  if (!int_category) {
    std::cout << name << ": " << msg << std::endl;
  } else {
    if (msg) {
      int_category->debug(name + "::" + msg);
    }
  }
}

void LoggingCategory::error(const char * msg) {
  if (!int_category) {
    std::cout << name << ": " << msg << std::endl;
  } else {
    if (msg) {
      int_category->error(name + "::" + msg);
    }
  }
}

void LoggingCategory::info(const char * msg) {
  if (!int_category) {
    std::cout << name << ": " << msg << std::endl;
  } else {
    if (msg) {
      int_category->info(name + "::" + msg);
    }
  }
}

void LoggingCategory::debug(int msg) {
  if (!int_category) {
    std::cout << name << ": " << msg << std::endl;
  } else {
    int_category->debug(name + "::" + std::to_string(msg));
  }
}

void LoggingCategory::error(int msg) {
  if (!int_category) {
    std::cout << name << ": " << msg << std::endl;
  } else {
    int_category->error(name + "::" + std::to_string(msg));
  }
}

void LoggingCategory::info(int msg) {
  if (!int_category) {
    std::cout << name << ": " << msg << std::endl;
  } else {
    int_category->info(name + "::" + std::to_string(msg));
  }
}

void LoggingCategory::debug(float msg) {
  if (!int_category) {
    std::cout << name << ": " << msg << std::endl;
  } else {
    int_category->debug(name + "::" + std::to_string(msg));
  }
}

void LoggingCategory::error(float msg) {
  if (!int_category) {
    std::cout << name << ": " << msg << std::endl;
  } else {
    int_category->error(name + "::" + std::to_string(msg));
  }
}

void LoggingCategory::info(float msg) {
  if (!int_category) {
    std::cout << name << ": " << msg << std::endl;
  } else {
    int_category->info(name + "::" + std::to_string(msg));
  }
}

void LoggingCategory::debug(double msg) {
  if (!int_category) {
    std::cout << name << ": " << msg << std::endl;
  } else {
    int_category->debug(name + "::" + std::to_string(msg));
  }
}

void LoggingCategory::error(double msg) {
  if (!int_category) {
    std::cout << name << ": " << msg << std::endl;
  } else {
    int_category->error(name + "::" + std::to_string(msg));
  }
}

void LoggingCategory::info(double msg) {
  if (!int_category) {
    std::cout << name << ": " << msg << std::endl;
  } else {
    int_category->info(name + "::" + std::to_string(msg));
  }
}
