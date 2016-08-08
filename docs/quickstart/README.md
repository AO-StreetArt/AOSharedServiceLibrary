# AO Shared Service Library Quick Start Guide

## Setup

Before we begin, we need to build our dependencies and then build the project.

### Dependencies

#### Ubuntu 14.04
The build_deps.sh script should allow for automatic resolution of dependencies.  Run the following commands from within the main folder

`mkdir ../aossl_deps`

`sudo cp scripts/deb/build_deps.sh ../aossl_deps`

`cd ../aossl_deps`

`sudo ./build_deps.sh`

#### Other
Please refer to the [Dependency Resolution] (https://github.com/AO-StreetArt/AOSharedServiceLibrary/tree/master/docs/deps) section of the documentation.

### Build the Project

The project and tests can be built with make on most linux systems.

`make`

`make test`

`sudo make install`

## Use
Please continue on to the [Use] (https://github.com/AO-StreetArt/AOSharedServiceLibrary/tree/master/docs/use) section of the documentation to see example uses of the library.

[Go Home] (https://github.com/AO-StreetArt/AOSharedServiceLibrary)
