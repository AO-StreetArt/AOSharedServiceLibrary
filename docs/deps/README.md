# Dependency Resolution

You will need Zero MQ which can be found [here] (http://zeromq.org/intro:get-the-software). Be sure to get the [C++ Drivers] (https://github.com/zeromq/cppzmq) in addition to the software.

For logging, we use log4cpp, which can be found [here] (http://log4cpp.sourceforge.net/)

You will need libhiredis, which can be found [here] (https://github.com/redis/hiredis)

You will need Couchbase drivers installed.  Couchbase drivers can be found [here] (http://developer.couchbase.com/documentation/server/4.1/sdks/c-2.4/overview.html)

You will need libmongoc and libbson for Mongo connections, follow the instructions [here] (http://mongoc.org/libmongoc/1.3.0/installing.html)

We rely also on a set of base libraries which are installable via most package managers:
* libuuid
* libcurl
* libevent

You will need [Hayai] (https://github.com/nickbruun/hayai) to run the benchmarks.

# External Services

We use several external services which are necessary to use the respective elements of this library.  Note that the elements of this library can be used separately, so you are in no way tied to the use of these services as a part of your project.  In fact, you are encouraged to develop drivers for alternate technologies and submit a pull request.

* [Redis] (http://redis.io/) - Key/Value Store
* [Couchbase] (http://www.couchbase.com/) - Document Based Database
* [Mongo] (https://www.mongodb.com) - Document Based Database
* [Consul] (https://www.consul.io/) - Service Discovery, Distributed Configuration, Health Monitoring


[Go Home] (https://github.com/AO-StreetArt/AOSharedServiceLibrary)
