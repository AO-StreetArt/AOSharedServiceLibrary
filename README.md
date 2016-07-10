# Project CLyman

## Build Status
* Project - Successful
* Tests - Successful

## Overview

This is a C++ Framework for Construction Microservices.  It is meant to provide a number of objects designed to make constructing microservices easier.  It is meant to be deployed as a static library within different microservice projects.

Features:
* Couchbase Administrator with cluster support
* Redis Administrator with cluster support
* HTTP Request Administrator
* Logging Module
* UUID Generator
* Outbound ZMQ Socket

## Use
The library is designed to be easy to use, and examples can be found in the test file for each object.

The Redis, HTTP, and ZMQ code is built to work with std::string, and the Couchbase Administrator works with objects that extend the interface 'writeable.h', found in the include directory.

The project and tests can be built with the provided scripts on Ubuntu 14.04 or Debian 7.

##### Dependency Resolution

For Ubuntu 14.04 & Debian 7, the build_deps.sh script should allow for automatic resolution of dependencies.  However, links are still included below for those who wish to build on other Operating Systems.

You will need RapidJSON which can be found [here] (https://github.com/miloyip/rapidjson).

Next, you will need Zero MQ which can be found [here] (http://zeromq.org/intro:get-the-software). Be sure to get the [C++ Drivers] (https://github.com/zeromq/cppzmq) in addition to the software.

For logging, we use log4cpp, which can be found [here] (http://log4cpp.sourceforge.net/)

You will also need xRedis, which can be found [here] (https://github.com/0xsky/xredis)

You will need Couchbase drivers installed.  Couchbase drivers can be found [here] (http://developer.couchbase.com/documentation/server/4.1/sdks/c-2.4/overview.html)
