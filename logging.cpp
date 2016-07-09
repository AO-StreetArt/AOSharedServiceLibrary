#include "logging.h"

log4cpp::Category *logging;

void end_log()
{
log4cpp::Category::shutdown();
}
