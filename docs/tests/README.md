# Tests

`make test`

Now we have a set of executables that end with "test" which run some basic tests on the libraries to ensure they function correctly.

    ./cli_test -name=test
    ./consul_test
    ./couchbase_test
    ./http_test
    ./logging_test
    ./redis_test
    ./uuid_test
    ./zmqio_test

Please note that the Redis, Couchbase, and Consul tests will require an active server/agent for the respective service.

# Benchmarks

`make benchmarks`

Now, we have a set of benchmark programs which are built on the [hayai] (https://github.com/nickbruun/hayai) framework.
These systematically test each element within the framework to provide realistic benchmarks on any given system.

    ./consul_benchmark
    ./couchbase_benchmark
    ./http_benchmark
    ./logging_benchmark
    ./redis_benchmark
    ./uuid_benchmark
    ./zmqio_benchmark

# Where to Go From Here
Please continue on to the [Dependencies] (https://github.com/AO-StreetArt/AOSharedServiceLibrary/tree/master/docs/deps) section of the documentation to see a discussion of the libraries dependencies for building on other operating systems.

See the automatically generated [API Documentation] (https://github.com/AO-StreetArt/AOSharedServiceLibrary/tree/master/docs/html) for detailed documentation on the library.


[Go Home] (https://github.com/AO-StreetArt/AOSharedServiceLibrary)
