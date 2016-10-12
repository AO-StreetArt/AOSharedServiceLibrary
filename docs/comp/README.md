# Compiling your Application

g++ is the recommended compiler for building applications using AOSSL, with a version supporting c++11.

## Linking

When linking your to AOSSL, you will also need to link to certain dependencies in many cases.  In all cases, you will need to link to aossl directly:

    -laossl

### HTTP Administrator

The HTTP Admin uses Curl, which can be linked against with:

    -lcurl

### Logging

For logging, we use the Log4Cpp module, which can be linked against with:

    -lpthread -llog4cpp

### UUID Generation
The UUID Generator uses libuuid, which can be linked against with:

    -luuid

### ZMQ
ZeroMQ Communications utilize a separate library, which can be linked against with:

    -lzmq

### Couchbase
Couchbase utilizes a separate library, which can be linked against with:

    -lcouchbase

### Redis
Redis utilizes the hiredis library, which can be linked against with:

    -lhiredis

If pkg-config is set up and libhiredis is installed correctly, we can also use:

    `pkg-config --cflags --libs hiredis`

## Standard

AO Shared Service Library is coded to the c++11 standard

## Tests
Please continue on to the [Tests] (https://github.com/AO-StreetArt/AOSharedServiceLibrary/tree/master/docs/tests) section of the documentation to learn about the libraries automated tests.

[Go Home] (https://github.com/AO-StreetArt/AOSharedServiceLibrary)
