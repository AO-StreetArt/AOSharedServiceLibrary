# Dependency Resolution

You will need Zero MQ which can be found [here] (http://zeromq.org/intro:get-the-software). Be sure to get the [C++ Drivers] (https://github.com/zeromq/cppzmq) in addition to the software.

For logging, we use log4cpp, which can be found [here] (http://log4cpp.sourceforge.net/)

You will also need xRedis, which can be found [here] (https://github.com/0xsky/xredis).  We recommend that you download our [development fork](https://github.com/AO-StreetArt/xredis), as we test any new pushes to the library on the dev branch and promote to master once it is considered stable.

You will need Couchbase drivers installed.  Couchbase drivers can be found [here] (http://developer.couchbase.com/documentation/server/4.1/sdks/c-2.4/overview.html)

We rely also on a set of base libraries which are installable via Ubuntu/Debian package managers:
* libuuid
* libcurl

# External Services

We use three external services which are necessary to use the respective elements of this library.  Note that the elements of this library can be used separately, so you are in no way tied to the use of these services as a part of your project.  In fact, you are encouraged to develop drivers for alternate technologies and submit a pull request.

* [Redis] (http://redis.io/) - Key/Value Store
* [Couchbase] (http://www.couchbase.com/) - Document Based Database
* [Consul] (https://www.consul.io/) - Service Discovery, Distributed Configuration, Health Monitoring


[Go Home] (https://github.com/AO-StreetArt/AOSharedServiceLibrary)
