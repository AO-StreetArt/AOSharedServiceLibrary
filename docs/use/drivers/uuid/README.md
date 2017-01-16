# UUID Generation

Easy and quick Universally Unique ID Generation:

    #include "include/factory_uuid.h"
    #include "include/factory/uuid_interface.h"

    uuidComponentFactory uuid_factory;
    uuidInterface *uuid = uuid_factory.get_uuid_interface();

    std::string uuid_str = uuid->generate();
