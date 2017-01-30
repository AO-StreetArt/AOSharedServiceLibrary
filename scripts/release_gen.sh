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

printf "Generating AOSSL Release: $RELEASE_NAME - $RELEASE_VERSION"
printf "Dependency Script Location: $DEPS_SCRIPT_LOC"

#Execute Doxygen to ensure we have the latest documentation generated
printf "Generating Fresh Doxygen Documentation"
cd .. && doxygen

#Make the library .a file
printf "Building library"
make

#Generate the release folder & subfolders
printf "Generating Release Folders"
mkdir ../$RELEASE_NAME
mkdir ../$RELEASE_NAME/deps
mkdir ../$RELEASE_NAME/examples
mkdir ../$RELEASE_NAME/examples/hello_world
mkdir ../$RELEASE_NAME/aossl
mkdir ../$RELEASE_NAME/aossl/include
mkdir ../$RELEASE_NAME/aossl/include/factory

#Copy the correct dependency script into the release subfolder 'deps'
printf "Copying Dependencies script into release folders"
cp scripts/$DEPS_SCRIPT_LOC/build_deps.sh ../$RELEASE_NAME/deps

#Copy the library and .a file into the release folder with the correct hierarchy
printf "Copying library into release folders"

#Header files
cp libaossl.a ../$RELEASE_NAME
cp makefile ../$RELEASE_NAME
cp LICENSE.txt ../$RELEASE_NAME
cp CODE_OF_CONDUCT.md ../$RELEASE_NAME

#Library files
cp aossl/*.cpp ../$RELEASE_NAME/aossl
cp aossl/include/*.h ../$RELEASE_NAME/aossl/include
cp aossl/include/factory/*.h ../$RELEASE_NAME/aossl/include/factory

#Examples
cp examples/hello_world/* ../$RELEASE_NAME/examples/hello_world

#Documentation
cp -r docs ../$RELEASE_NAME

#Copy the easy install script into the release folder
cp scripts/easy_install.sh ../$RELEASE_NAME

#Copy the Installation Make file into the release folder
cp scripts/make_install_release ../$RELEASE_NAME

#Generate the Release tar files
printf "Generate Release Tar Files"
tar -czvf $RELEASE_NAME-$RELEASE_VERSION.tar.gz ../$RELEASE_NAME
tar -cjvf $RELEASE_NAME-$RELEASE_VERSION.tar.bz2 ../$RELEASE_NAME
zip -r $RELEASE_NAME-$RELEASE_VERSION.zip ../$RELEASE_NAME

#Move the tar files outside the main git repository
mv *.tar.* ../
mv *.zip ../

printf "Script finished, release folders can be found at ../../$RELEASE_NAME, tar files at ../.."

exit 0
