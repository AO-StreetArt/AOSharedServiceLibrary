# AO Shared Service Library

![](https://travis-ci.org/AO-StreetArt/AOSharedServiceLibrary.svg?branch=master)

## Overview

Welcome to the AO Shared Service Library.  This is a framework designed for constructing C++ Microservices.
This means smaller, more focused programs that perform a single function well and rely on inter-process
communications to perform complex tasks.  

While designed with a Microservice architecture in mind, AOSSL also seeks to maintain enough flexibility to allow for use with any program architecture and any other libraries.  AOSSL uses a plug-and-play architecture to allow users to pick and choose components they would like to use, and exclude others.  This is meant to reduce time-to-market on C++ Services, and is designed to be easy to use and reliable, while maintaining high performance standards.

### Features:

#### Connections to Critical External Tools

* Consul Interface

#### Basic Tools
* Asynchronous Logging Module
* Universally Unique ID Generator
* Command Line Argument Parser
* Properties File Parser

#### Platform Support
* Support for Ubuntu 14.04, Ubuntu 16.04, Debian 7, Debian 8, CentOS 7, RHEL 7
* Easy to construct Docker Images with templates

## Documentation
Check out online documentation of the latest version at [ReadTheDocs](http://aosharedservicelibrary.readthedocs.io/en/latest/index.html).

Stuck and need help?  Have general questions about the library?  Reach out to the development team at aossl@emaillist.io

## Contributing

All contributors are welcome, including (but not limited to) pull requests, issues, documentation, examples, and feature requests.  Please see the Code of Conduct and the contributing files for an overview on the expectations for all contributors.

Contributors should also review the documentation carefully, in particular the [Developer Notes](http://aosharedservicelibrary.readthedocs.io/en/latest/dev_notes.html) section.

We are actively looking for anyone interested in working on the following areas:

* Security
* Thread-Safety
* Code Optimization

If you are interested, or are interested in joining the development team in general, please send an email to aossl@emaillist.io
