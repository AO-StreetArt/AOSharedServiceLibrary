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

The HTTP Admin allows quick and easy HTTP Requests by exposing:

* bool put(char * url, char * data, int timeout)
* bool get(char * url, int timeout)
* bool post(char * url, char * data, int timeout)
* bool del(char * url, int timeout)

It also allows binding a callback to return when the data is recieved from a get request, by calling bind_get_callback(func)

    //------------------------------SETUP-------------------------------------//

    //A String to store response data
    std::string writedata;

    //This is the callback that gets called when we recieve the response to the
    //Get Curl Request
    size_t writeCallback(char * buf, size_t size, size_t nmemb, void* up)
    {

      logging->debug("Callback Triggered");

    //Put the response into a string
    for (int c = 0; c<size*nmemb; c++)
    {
      writedata.push_back(buf[c]);
    }

    return size*nmemb;
    }

    //Declare the admin
    HttpAdmin ha;

    //-------------------------------GET--------------------------------------//

    writedata.clear();

    ha.bind_get_callback(writeCallback);

    //Send the request
    bool success = ha.get(GETURL, 5);
    if (!success) {
    }
    else {
      logging->debug("Retrieved:");
      logging->debug(writedata);
    }

    //-------------------------------PUT--------------------------------------//
    success = ha.put(PUTURL, "123", 5);

    //-------------------------------POST-------------------------------------//
    success = ha.post(POSTURL, "CLYMAN", 5);

    //------------------------------DELETE------------------------------------//
    success = ha.del(DELETEURL, 5);

## Logging
Logging exposes a pointer to a Logger instance, which can log directly itself or
provide categories to log to.

    std::string initFileName = "configuration_file";
    logging = new Logger(initFileName);

    logging->debug("My message");
    logging->get_category("MyCategory").error("My other message");

We read from the log4cpp configuration files, of which several examples are provided within the library itself.

## UUID Generation

Easy and quick Universally Unique ID Generation:

`uuidAdmin uuid;`

`std::string uuid_str = uuid.generate();``

## ZeroMQ Sockets

This provides Request/Reply Managers for both Inbound and Outbound sockets.

Both the Zmqo and Zmqi objects expose these methods:

* recv() - a blocking call to wait for a message
* send(const char * msg, int msg_size)
* send(std::string msg)

In order to connect to a socket, Zmqo exposes connect while Zmqi exposes bind:

    //We maintain the ZMQ Context and pass it to the ZMQ objects coming from aossl
    zmq::context_t context(1, 2);

    //Set up the outbound ZMQ Client
    zmqo = new Zmqo (context);
    zmqo->connect("tcp://localhost:5555");

    //Set up the inbound ZMQ Client
    zmqi = new Zmqi (context);
    zmqi->bind("tcp://*:5555");

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
