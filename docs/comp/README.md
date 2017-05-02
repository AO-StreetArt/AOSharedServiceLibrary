# Compiling your Application

g++ is the recommended compiler for building applications using AOSSL, version 5.0 or greater.

## Linking
When linking your to AOSSL, you will also need to link to certain dependencies in many cases.  In all cases, you will need to link to aossl directly:

    -laossl

### HTTP Administrator
The HTTP Admin uses Curl, which can be linked against with:

    -lcurl

### HTTP Server
The HTTP Server relies on libevent, which can be linked against with:

    -levent

### Consul Administrator
The Consul Admin uses the HTTP Administrator, so you will need to link against curl here as well:

    -lcurl

### Logging
For logging, we use the Log4Cpp module, which can be linked against with:

    -lpthread -llog4cpp

### UUID Generation
The UUID Generator uses libuuid, which can be linked against with:

    -luuid

### ZMQ
ZeroMQ Communications utilize the ZMQ library and C++ bindings, which can be linked against with:

    -lzmq

### Redis
Redis utilizes the hiredis library, which can be linked against with:

    -lhiredis

If pkg-config is set up and libhiredis is installed correctly, we can also use:

    `pkg-config --cflags --libs hiredis`

### Mongo
Mongo utilizes both the Mongo C Driver libmongoc as well as libbson for json to bson conversions.

    -lmongoc-1.0 -lbson-1.0

### Neo4j
Neo4j depends on the Neo4j client, which needs to link against several dependencies:

    -lneo4j-client -lssl -lcrypto -lm

## Standard

AO Shared Service Library is coded to the c++11 standard.

-std=c++11

## Tests
Please continue on to the [Tests](https://github.com/AO-StreetArt/AOSharedServiceLibrary/tree/master/docs/tests) section of the documentation to learn about the libraries automated tests.

[Go Home](https://github.com/AO-StreetArt/AOSharedServiceLibrary)
