# How to Use the AO Shared Service Library

The library is designed to be easy to use, and examples can be found in the test file for each object.

## Couchbase Administrator

The Couchbase Administrator works with objects that extend the interface 'writeable.h', found in the include directory.

Let's look at the couchbase_test.cpp file.

`class TestData: public Writeable`

We start by defining a class which extends the Writeable interface.  This requires only that we have three
methods implemented:

* get_key
* set_key
* to_json



## Redis Administrator

## Consul Administrator

The Consul Admin is responsible for Service Registration & Discovery, Key-Value
Retrieval, and Health Check Configuration.

We first define a Service which represents the current instance of the code.

This contains an ID, a name, a connection, and a port number.  Tags can be added or not:

`Service s ("1", "test", "http://localhost/", "5555");`
`s.add_tag("Testing");`

And build our Consul Admin, specifying the connection address:

`ConsulAdmin ca ("localhost:8500");`

Now, we can register the currently running instance of a service with Consul

`ca.register_service(s);`

And, we can unregister on shutdown:

`ca.deregister_service(s);`

We can get and set key-value storage elements:

`bool success = ca.set_config_value("Test", "123");`
`assert(success);`

`std::string test_val = ca.get_config_value("Test");`
`logging->debug(test_val);`

It also has some limited query capabilities, please see API for further details.

## HTTP Administrator

## Logging

## UUID Generation

Easy and quick Universally Unique ID Generation:

`uuidAdmin uuid;`

`std::string uuid_str = uuid.generate();``

## ZeroMQ Sockets

## Command Line Argument Parser

The CommandLineInterpreter makes working with input parameters easier.  With it,
we get access to command line arguments in the form:

`./example -name=abc`

We have access to an opt_exist method to determine if an option was entered.  We can also use get_opt to pull parameter values, and get_program_name to return the current program name executing on this instance.

    int main( int argc, char** argv )
    {

    CommandLineInterpreter cli ( argc, argv );
    std::cout << cli.get_program_name() << std::endl;
    if ( cli.opt_exist("name") ) {
      std::cout << cli.get_opt("name") << std::endl;
    }

    return 0;
    } 
