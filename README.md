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
* Universally Unique ID Generator
* Inbound & Outbound Request/Response ZeroMQ Socket managers
* A Command Line Argument Parser

In order to use the respective administrators, you should have an active [Consul] (https://www.consul.io/) Agent, as well as servers of [Couchbase] (http://www.couchbase.com/) and [Redis] (http://redis.io/) to connect to.

Full API Documentation is available via doxygen, and the latest execution is stored in the docs/html folder.

## Table of Contents

* [Quickstart] (https://github.com/AO-StreetArt/AOSharedServiceLibrary/tree/master/docs/quickstart)
* [Use] (https://github.com/AO-StreetArt/AOSharedServiceLibrary/tree/master/docs/use)
* [Dependency Resolution] (https://github.com/AO-StreetArt/AOSharedServiceLibrary/tree/master/docs/deps)
* [Tests] (https://github.com/AO-StreetArt/AOSharedServiceLibrary/tree/master/docs/tests)
* [Building Documentation] (https://github.com/AO-StreetArt/AOSharedServiceLibrary/tree/master/docs)
