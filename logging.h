#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <string>
#include <stdlib.h>

#ifndef LYMAN_LOG
#define LYMAN_LOG

extern log4cpp::Category *logging;

void end_log();

#endif
