# AO Shared Service Library Quick Start Guide

## Docker

The AO Shared Service Library is available as a docker image.  This allows developers to get a fully functional build environment with a single command, and offers a head-start on making production docker files.  Docker is the recommended method of deployment for Services, as it allows for easy horizontal scaling.

### Setup

Before we begin, we need to have a few things ready.

#### Installing Docker

If you do not already have Docker installed, please follow the instructions [here] (https://docs.docker.com/engine/installation).

### Running the Docker Image

You can create a fully functional build environment for new micro services via Docker, and ssh into that process.

`docker run --name aossl -d -P aostreetart/ao-services`

Then, you can access the container with the following:

`sudo docker exec -i -t aossl /bin/bash`

### Docker Images of External Tools

Docker images are also available for many of the external tools connected to within the library.

#### Mongo

In times when you need to connect to an instance of [Mongo] (https://www.mongodb.com), you can use the docker image (full instructions can be found [here] (https://hub.docker.com/_/mongo/)).

    docker run --name some-mongo -d mongo

#### Neo4j

In times when you need to connect to an instance of [Neo4j] (https://neo4j.com/), you can use the docker image (full instructions can be found [here] (https://hub.docker.com/_/neo4j/)).

    docker run \
        --publish=7474:7474 --publish=7687:7687 \
        --volume=$HOME/neo4j/data:/data \
        neo4j

#### Redis

In times when you need to connect to an instance of [Redis] (http://redis.io/), you can use the docker image (full instructions can be found [here] (https://hub.docker.com/_/redis/)).

`docker run --name some-redis -d redis`

#### Consul

In times when you need to connect to an instance of [Consul] (https://www.consul.io/), you can use the docker image (full instructions can be found [here] (https://hub.docker.com/_/consul/))

`docker run -d --name=dev-consul consul`

### Connecting Docker Images

In order to connect a docker image of AOSSL with it's respective component, we can use the --link option when we start it's container.  For example:

    docker run --name some-mongo -d mongo
    docker run --name aossl -d -P aostreetart/ao-services
    docker run --name aossl --link some-mongo:mongo -d -P aostreetart/ao-services

## Use Latest Release

Please see the [releases] (https://github.com/AO-StreetArt/AOSharedServiceLibrary/releases) page to download the latest release of the library.  Once downloaded, unpack the tar/zip file and cd into the main directory.  Then, run the following command:

    sudo ./easy_install

You will be prompted for your sudo password, after which the script will attempt to install all of the necessary dependencies, and then the library itself.  If you prefer, you can simply run:

    sudo make install

This will install the library without installing the dependencies.  You may execute the install dependencies script separately if desired via:

    cd deps && sudo ./build_deps.sh

You may uninstall the library by executing:

    sudo make uninstall

## Build from Source

While Docker is a faster alternative, we can also build from source.  Note that this is currently only recommended on Unix systems due to OS-level dependencies.

### Setup

Before we begin, we need to build our dependencies and then build the project.

#### Dependencies

##### Ubuntu 14.04/Debian 7
The build_deps.sh script should allow for automatic resolution of dependencies.  Run the following commands from within the main folder

`mkdir ../aossl_deps`

`sudo cp scripts/deb/build_deps.sh ../aossl_deps`

`cd ../aossl_deps`

`sudo ./build_deps.sh`

##### CentOS 7/Redhat Enterprise Linux 7
The build_deps.sh script should allow for automatic resolution of dependencies.  Run the following commands from within the main folder

`mkdir ../aossl_deps`

`sudo cp scripts/rhel/build_deps.sh ../aossl_deps`

`cd ../aossl_deps`

`sudo ./build_deps.sh`

##### Other
Please refer to the [Dependency Resolution] (https://github.com/AO-StreetArt/AOSharedServiceLibrary/tree/master/docs/deps) section of the documentation.

#### Build the Project

The project and tests can be built with make on most linux systems.

`make`

We can clean the build and remove all generated files with:

`make clean`

#### Build the Tests/Benchmarks

##### Ubuntu 14.04/Debian 7

Run the following to build the library test executables and the benchmarking apps

    make tests
    make benchmarks

##### CentOS 7/Redhat Enterprise Linux 7

Run the following to build the library test executables and the benchmarking apps

    make rhel-test
    make rhel-benchmarks

#### Install and Uninstall the Project

The project can be installed on linux systems with:

`sudo make install`

We can uninstall the libraries with:

`sudo make uninstall`

## Use
Please continue on to the [Use] (https://github.com/AO-StreetArt/AOSharedServiceLibrary/tree/master/docs/use) section of the documentation to see example uses of the library.

[Go Home] (https://github.com/AO-StreetArt/AOSharedServiceLibrary)
