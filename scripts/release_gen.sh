#!/bin/bash
#This script will install the pre-packaged release of AOSSL

#Pull the command line parameters
#The generated tar file will have the name RELEASE_NAME-RELEASE_VERSION.tar.gz
#The dependencies script specified in the third option will be packaged with the library
if [ "$#" -ne 3 ]; then
    echo "The correct usage of this script is './release_gen.sh release-name release-version dependency-script-location'"
    exit 64
fi
RELEASE_NAME=$1
RELEASE_VERSION=$2
DEPS_SCRIPT_LOC=$3

#Make the library .a file
cd .. && make

#Generate the release folder & subfolders
mkdir ../../$RELEASE_NAME
mkdir ../../$RELEASE_NAME/deps
mkdir ../../$RELEASE_NAME/examples
mkdir ../../$RELEASE_NAME/docs

#Copy the correct dependency script into the release subfolder 'deps'

#Copy the library and .a file into the release folder with the correct hierarchy

#Copy the easy install script into the release folder

#Generate the Release tar file
