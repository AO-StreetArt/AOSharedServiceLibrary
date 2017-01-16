# Consul Administrator

Consul is responsible for Service Registration & Discovery, as well as Key-Value Retrieval.

We start by importing the necessary interfaces and the establish the service factory

    #include "include/factory/consul_interface.h"
    #include "include/factory_consul.h"

    ConsulComponentFactory consul_factory;

Next, we define a Service which represents the current instance of the code.

This contains an ID, a name, a connection, and a port number.  Tags can be added or not:

    ServiceInterface *s = consul_factory.get_service_interface("1", "test", "http://localhost/", "5555");
    s->add_tag("Testing");

And build our Consul Admin, specifying the connection address:

    ConsulInterface *ca = consul_factory.get_consul_interface("localhost:8500");

Now, we can register the currently running instance of a service with Consul

    ca->register_service(*s);

And, we can unregister on shutdown:

    ca->deregister_service(*s);

We can get and set key-value storage elements:

    bool success = ca->set_config_value("Test", "123");
    assert(success);

    std::string test_val = ca->get_config_value("Test");
    logging->debug(test_val);

It also has some limited query capabilities, please see API for further details.
