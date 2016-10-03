# AO Shared Service Library

## Build Status
* Project - Successful
* Tests - Successful

## Overview

Welcome to the AO Shared Service Library.  This is a framework for constructing C++ Microservices.
This means smaller, more focused programs that perform a single function well and rely on inter-process
communications.  

### Why use AOSSL?

Over the last twenty years, most enterprise scale applications have been built as 'monoliths', a single,
large piece of software that must be deployed all at once.  This has a host of limitations, such as scalability
and difficulty of highly-available systems.

Microservices offer a different design, instead relying on many different programs that all communicate
in order to provide all of the services once provided by the monolithic application.  This lets us
scale by starting up more instances of programs, and introduces a system with no single point of failure.

The AO Shared Service Library provides frameworks for microservices, ranging from communications protocols to
useful tools all services need.  This is meant to reduce time-to-market on C++ Services, and is designed
to be reliable and robust while maintaining high performance standards.

It also provides a plug-and-play architecture, allowing users to pick elements of the framework they would
like to use and exclude others.  This also promotes extensibility, as careful interface design allows for
rapid development of new capabilities.

### Features:
#### Core Frameworks revolving around inter-service messaging protocols
* HTTP Service Framework
* ZeroMQ Service Framework

#### Connections to Critical External Tools
* Couchbase Administrator with cluster support
* Redis Administrator
* Consul Administrator

#### Basic Tools that all good services need
* Asynchronous Logging Module
* Universally Unique ID Generator
* A Command Line Argument Parser

#### Platform Support
* Support for Ubuntu 14.04, Debian 7, CentOS 7, RHEL 7
* Easy to construct Docker Images with templates

In order to use the respective administrators, you should have an active [Consul] (https://www.consul.io/) Agent, as well as servers of [Couchbase] (http://www.couchbase.com/) and [Redis] (http://redis.io/) to connect to.

Full API Documentation is available via doxygen, and the latest execution is stored in the docs/html folder.

## Table of Contents

* [Quickstart] (https://github.com/AO-StreetArt/AOSharedServiceLibrary/tree/master/docs/quickstart)
* [Use] (https://github.com/AO-StreetArt/AOSharedServiceLibrary/tree/master/docs/use)
* [Dependency Resolution] (https://github.com/AO-StreetArt/AOSharedServiceLibrary/tree/master/docs/deps)
* [Tests] (https://github.com/AO-StreetArt/AOSharedServiceLibrary/tree/master/docs/tests)
* [Building Documentation] (https://github.com/AO-StreetArt/AOSharedServiceLibrary/tree/master/docs)
