#!/bin/bash
#This script will install the pre-packaged release of AOSSL

printf "Attempting to install dependencies"

#Install the dependencies
cd ./deps && sudo ./build_deps.sh

printf "Attempting to install core library"

# Install the library
cd ../ && sudo make install

printf "Finished installing AOSSL"
