#!/bin/bash
exec 3>&1 4>&2
trap 'exec 2>&4 1>&3' 0 1 2 3
exec 1>build_deps.log 2>&1

#This script will attempt to build basic project dependencies

#Based on CentOS 7

PRE=./downloads
RETURN=..
mkdir $PRE

# Update the Server
sudo yum -y update

# Build & Install the Shared Service Library

if [ ! -d /usr/local/include/aossl ]; then

  # Create the folder to clone into
  mkdir $PRE/aossl

  # Get the latest code from the master branch on github
  # You may alternatively download a release from the github releases page and use that
  git clone https://github.com/AO-StreetArt/AOSharedServiceLibrary.git $PRE/aossl

  #Build the dependencies for the shared service library
  mkdir $PRE/aossl_deps
  cp $PRE/aossl/scripts/rhel/build_deps.sh $PRE/aossl_deps/
  cd $PRE/aossl_deps && sudo ./build_deps.sh
  cd ../$RETURN

  #Build the shared service library
  cd $PRE/aossl && make && sudo make install
  sudo ldconfig

fi

# Here we look to install RapidJSON

# This is a recommended library for JSON Processing.
# Libprotobuf and protoc are also installed by default, for using Google Protocol Buffers.
# If you wish to use other parsing methods or message formats, simply remove these
if [ ! -d /usr/local/include/rapidjson ]; then

  printf "Cloning RapidJSON"

  mkdir $PRE/rapidjson

  #Get the RapidJSON Dependency
  git clone https://github.com/miloyip/rapidjson.git $PRE/rapidjson

  #Move the RapidJSON header files to the include path
  sudo cp -r $PRE/rapidjson/include/rapidjson/ /usr/local/include

fi

#Install protobuf and the protobuf compiler
sudo yum install -y protobuf-devel protobuf-compiler

printf "Finished installing dependencies"
