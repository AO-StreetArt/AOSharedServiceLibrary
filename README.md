# AO Shared Service Library

![](https://travis-ci.org/AO-StreetArt/AOSharedServiceLibrary.svg?branch=master)

## Overview

Welcome to the AO Shared Service Library.  This is a framework designed for constructing C++ Microservices.
This means smaller, more focused programs that perform a single function well and rely on inter-process
communications to perform complex tasks.  

While designed with a Microservice architecture in mind, AOSSL also seeks to maintain enough flexibility to allow for use with any program architecture and any other libraries.  AOSSL uses a plug-and-play architecture to allow users to pick and choose components they would like to use, and exclude others.  This is meant to reduce time-to-market on C++ Services, and is designed to be easy to use and reliable, while maintaining high performance standards.

### Features:
#### Core Frameworks revolving around inter-service messaging protocols
* HTTP Service Framework
* ZeroMQ Service Framework

#### Connections to Critical External Tools

* Mongo Interface
* Redis Interface
* Consul Interface
* Neo4j Interface

#### Basic Tools
* Asynchronous Logging Module
* Universally Unique ID Generator
* Command Line Argument Parser
* Properties File Parser

#### Platform Support
* Support for Ubuntu 14.04, Ubuntu 16.04, Debian 7, Debian 8, CentOS 7, RHEL 7
* Easy to construct Docker Images with templates

#### Backwards Compatibility
Starting from the release of v1.0, backwards compatibility with future versions is guaranteed up to v2.0, provided users code to the interfaces (found in lib/include/factory) rather than the implementation files.  Examples may be found in the 'Use' section of the documentation.

Any interface breaking changes within the next major version will be clearly documented with the release notes.

In order to use the respective interfaces, you should have an active [Consul] (https://www.consul.io/) Agent, as well as servers of [Neo4j] (https://neo4j.com/), [Redis] (http://redis.io/), and/or [Mongo] (https://www.mongodb.com) to connect to.

Full API Documentation is available via doxygen, and the latest execution is stored in the docs/html folder.

## Table of Contents

* [Quickstart] (https://github.com/AO-StreetArt/AOSharedServiceLibrary/tree/master/docs/quickstart)
* [Use] (https://github.com/AO-StreetArt/AOSharedServiceLibrary/tree/master/docs/use)
* [Dependency Resolution] (https://github.com/AO-StreetArt/AOSharedServiceLibrary/tree/master/docs/deps)
* [Tests] (https://github.com/AO-StreetArt/AOSharedServiceLibrary/tree/master/docs/tests)
* [Building Documentation] (https://github.com/AO-StreetArt/AOSharedServiceLibrary/tree/master/docs)

## Contributing

All contributors are welcome, including (but not limited to) pull requests, issues, documentation, examples, and feature requests.  Please see the Code of Conduct file for an overview on the expectations for all contributors.

There are a number of topics which have been identified as areas needing development, and contributors are encouraged to work on these areas as well.  

* Sentinel & Cluster support for Redis Drivers
* Benchmarks for Mongo & Neo4j
* Expanded ZeroMQ Pub/Sub Architecture automated tests
* Library testing with OSX and/or other Linux Distributions not already supported
* CMake Support
