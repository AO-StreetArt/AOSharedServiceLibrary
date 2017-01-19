#!/bin/bash
#This script will install the pre-packaged release of AOSSL

#Pull the command line parameters
#The generated tar file will have the name RELEASE_NAME-RELEASE_VERSION.tar.gz
#The dependencies script in the folder specified in the third option will be packaged with the library
if [ "$#" -ne 3 ]; then
    echo "The correct usage of this script is './release_gen.sh release-name release-version dependency-script-location'"
    exit 64
fi
RELEASE_NAME=$1
RELEASE_VERSION=$2
DEPS_SCRIPT_LOC=$3

#Execute Doxygen to ensure we have the latest documentation generated
cd .. && doxygen

#Make the library .a file
cd .. && make

#Generate the release folder & subfolders
mkdir ../../$RELEASE_NAME
mkdir ../../$RELEASE_NAME/deps
mkdir ../../$RELEASE_NAME/examples
mkdir ../../$RELEASE_NAME/examples/hello_world
mkdir ../../$RELEASE_NAME/lib
mkdir ../../$RELEASE_NAME/lib/include
mkdir ../../$RELEASE_NAME/lib/include/factory

#Copy the correct dependency script into the release subfolder 'deps'
cp $DEPS_SCRIPT_LOC/build_deps.sh ../../$RELEASE_NAME/deps

#Copy the library and .a file into the release folder with the correct hierarchy

#Header files
cp ../libaossl.a ../../$RELEASE_NAME
cp ../makefile ../../$RELEASE_NAME
cp ../LICENSE.txt ../../$RELEASE_NAME
cp ../CODE_OF_CONDUCT.md ../../$RELEASE_NAME

#Library files
cp ../lib/*.cpp ../../$RELEASE_NAME/lib
cp ../lib/include/*.h ../../$RELEASE_NAME/lib/include
cp ../lib/include/factory/*.h ../../$RELEASE_NAME/lib/include/factory

#Examples
cp ../examples/hello_world/* ../../$RELEASE_NAME/examples/hello_world

#Documentation
cp -r ../docs ../../$RELEASE_NAME

#Copy the easy install script into the release folder
cp easy_install.sh ../../$RELEASE_NAME

#Generate the Release tar files
tar -czvf $RELEASE_NAME-$RELEASE_VERSION.tar.gz ../../$RELEASE_NAME
tar -cjvf $RELEASE_NAME-$RELEASE_VERSION.tar.bz2 ../../$RELEASE_NAME

#Move the tar files outside the main git repository
mv *.tar.* ../../

exit 0
