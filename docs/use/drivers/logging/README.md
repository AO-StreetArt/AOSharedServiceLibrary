# Logging
Logging exposes a pointer to a Logger instance, which can log directly itself or
provide categories to log to.

    #include "include/aossl/logging/factory_logging.h"
    #include "include/aossl/logging/logging_interface.h"

    LoggingComponentFactory logging_factory;

    std::string initFileName = "configuration_file";
    logging = logging_factory.get_logging_interface(initFileName);

    logging->debug("My message");

We read from the [log4cpp configuration files] (http://log4cpp.sourceforge.net/api/classlog4cpp_1_1PropertyConfigurator.html), of which several examples are provided within the library itself.
