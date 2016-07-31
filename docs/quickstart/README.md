# Use
The library is designed to be easy to use, and examples can be found in the test file for each object.

The Redis, HTTP, and ZMQ code is built to work with std::string, and the Couchbase Administrator works with objects that extend the interface 'writeable.h', found in the include directory.

The project and tests can be built with the provided scripts on Ubuntu 14.04 or Debian 7.

`./build_project.sh`

At this point, you will have a few outputs, which can be copied to allow easy use across many projects (example commands provided for Ubuntu 14.04):

* A compiled shared library libaossl.a, that needs to be put on the linker path

`sudo cp libaossl.a /usr/local/bin`

* A set of header files in the lib/include directory that need to be put onto the include path

`sudo mkdir /usr/local/include/aossl`

`sudo cp lib/include/* /usr/local/include/aossl`
