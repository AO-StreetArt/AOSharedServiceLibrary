# AO Shared Service Library Quick Start Guide

## Setup

Before we begin, we need to build our dependencies and then build the project.

### Dependencies

#### Ubuntu 14.04
The build_deps.sh script should allow for automatic resolution of dependencies.  Run the following commands from within the CLyman folder

`mkdir ../CLyman_deps`

`sudo cp build_deps.sh ../CLyman_deps`

`cd ../CLyman_deps`

`sudo ./build_deps.sh`

#### Other
Please refer to the [Dependency Resolution] (https://github.com/AO-StreetArt/AOSharedServiceLibrary/tree/master/docs/deps) section of the documentation.

### Build the Project

The project and tests can be built with the provided scripts on Ubuntu 14.04 or Debian 7.

`./build_project.sh`

At this point, you will have a few outputs, which can be copied to allow easy use across many projects (example commands provided for Ubuntu 14.04):

* A compiled shared library libaossl.a, that needs to be put on the linker path

`sudo cp libaossl.a /usr/local/bin`

* A set of header files in the lib/include directory that need to be put onto the include path

`sudo mkdir /usr/local/include/aossl`

`sudo cp lib/include/* /usr/local/include/aossl`

## Use
The library is designed to be easy to use, and examples can be found in the test file for each object.

### Couchbase Administrator

The Couchbase Administrator works with objects that extend the interface 'writeable.h', found in the include directory.

### Redis Administrator

### Consul Administrator

### HTTP Administrator

### Logging

### UUID Generation

### ZeroMQ Sockets

### Command Line Argument Parser
