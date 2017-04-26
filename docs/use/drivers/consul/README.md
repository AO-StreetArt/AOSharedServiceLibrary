# Consul Administrator

Consul is responsible for Service Registration & Discovery, as well as Key-Value Retrieval.

The Consul Admin utilizes HTTP to communicate.  It is threadsafe, but has a mutex on the HTTP Send function, so only one value can be retrieved at a time.  For this reason, it's recommended that you retrieve all of your necessary Consul values at start up, rather than pulling them during program execution.

## Use

We start by importing the necessary interfaces and establish the service factory.

    #include "aossl/consul/consul_interface.h"
    #include "aossl/consul/factory_consul.h"

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

The Consul Admin also supports querying available services, nodes, and datacenters.  Please see the API documentation for available queries, as well as the [Consul HTTP API](https://www.consul.io/api/index.html) for details on expected responses.

All responses from the Consul Admin are strings which hold the JSON response from the Consul HTTP API. Configuration Values retrieved from the KV store in Consul are also returned as JSON, and the value returned is encoded in base64.  Please see the Consul documentation for further details.
