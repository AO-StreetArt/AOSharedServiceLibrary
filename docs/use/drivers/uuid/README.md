# UUID Generation

Easy and quick Universally Unique ID Generation:

    #include "aossl/uuid/factory_uuid.h"
    #include "aossl/uuid/uuid_interface.h"
    #include <iostream>

    uuidComponentFactory id_factory;
    //Initialize the UUID Admin
    uuidInterface *uuid = id_factory.get_uuid_interface();

    //Generate a UUID
    UuidContainer id_container = uuid->generate();
    std::cout << id_container.id << std::endl;
    std::cout << id_container.err << std::endl;