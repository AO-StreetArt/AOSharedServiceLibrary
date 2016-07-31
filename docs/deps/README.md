# Dependency Resolution

## Ubuntu 14.04 / Debian 7

For Ubuntu 14.04 & Debian 7, the build_deps.sh script should allow for automatic resolution of dependencies.  

## Other

Links are included below for those who wish to build on other Operating Systems.

You will need Zero MQ which can be found [here] (http://zeromq.org/intro:get-the-software). Be sure to get the [C++ Drivers] (https://github.com/zeromq/cppzmq) in addition to the software.

For logging, we use log4cpp, which can be found [here] (http://log4cpp.sourceforge.net/)

You will also need xRedis, which can be found [here] (https://github.com/0xsky/xredis).  We recommend that you download our [development fork](https://github.com/AO-StreetArt/xredis), as we test any new pushes to the library on the dev branch and promote to master once it is considered stable.

You will need Couchbase drivers installed.  Couchbase drivers can be found [here] (http://developer.couchbase.com/documentation/server/4.1/sdks/c-2.4/overview.html)

We rely also on a set of base libraries which are installable via Ubuntu/Debian package managers:
* libuuid
* libcurl
