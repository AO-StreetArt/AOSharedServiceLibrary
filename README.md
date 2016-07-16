# AO Shared Service Library

## Build Status
* Project - Successful
* Tests - Successful

## Overview

This is a C++ Framework for Constructing Microservices.  It is meant to provide a number of objects designed to make constructing a service easier.  It is meant to be deployed as a static library within different projects.

Features:
* Couchbase Administrator with cluster support
* Redis Administrator with cluster support
* Consul Administrator
* HTTP Request Administrator
* Asynchronous Logging Module
* UUID Generator
* Inbound & Outbound ZMQ Socket managers

In order to use the respective administrators, you should have an active [Consul] (https://www.consul.io/) Agent, as well as servers of [Couchbase] (http://www.couchbase.com/) and [Redis] (http://redis.io/) to connect to.

## Use
The library is designed to be easy to use, and examples can be found in the test file for each object.

The Redis, HTTP, and ZMQ code is built to work with std::string, and the Couchbase Administrator works with objects that extend the interface 'writeable.h', found in the include directory.

The project and tests can be built with the provided scripts on Ubuntu 14.04 or Debian 7.

`./build_project.sh`

At this point, you will have a few outputs, which can be copied to allow easy use across many projects (example commands provided for Ubuntu 14.04):

* A compiled shared library libaossl.a.x.y, where x and y are integers, that needs to be put on the linker path

`sudo cp libaossl.a.* /usr/local/bin`

* A set of header files in the lib/include directory that need to be put onto the include path

`sudo mkdir /usr/local/include/aossl`
`sudo cp lib/include/* /usr/local/include/aossl`

## Tests

`./build_tests.sh`

Now we have a set of executables that end with "test" which run some basic tests on the libraries to ensure they function correctly.

## Dependency Resolution

### Ubuntu 14.04 / Debian 7

For Ubuntu 14.04 & Debian 7, the build_deps.sh script should allow for automatic resolution of dependencies.  

### Other

Links are included below for those who wish to build on other Operating Systems.

You will need RapidJSON which can be found [here] (https://github.com/miloyip/rapidjson).

Next, you will need Zero MQ which can be found [here] (http://zeromq.org/intro:get-the-software). Be sure to get the [C++ Drivers] (https://github.com/zeromq/cppzmq) in addition to the software.

For logging, we use log4cpp, which can be found [here] (http://log4cpp.sourceforge.net/)

You will also need xRedis, which can be found [here] (https://github.com/0xsky/xredis)

You will need Couchbase drivers installed.  Couchbase drivers can be found [here] (http://developer.couchbase.com/documentation/server/4.1/sdks/c-2.4/overview.html)
