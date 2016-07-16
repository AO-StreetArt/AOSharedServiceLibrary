#!/bin/bash
exec 3>&1 4>&2
trap 'exec 2>&4 1>&3' 0 1 2 3
exec 1>build_deps.log 2>&1

#This script will attempt to build CLyman dependencies

#Based on Ubuntu 14.04 LTS
#Not intended for use with other OS (should function correctly with Debian 7, untested)

printf "Creating Dependency Folder"
PRE=./downloads
mkdir $PRE

printf "Calling apt-get update"

#Update the Ubuntu Server
sudo apt-get -y update

#Build the dependencies and place them in the correct places

printf "Addressing pre-build requirements"

#Ensure that specific build requirements are satisfied
sudo apt-get -y install build-essential libtool pkg-config autoconf automake uuid-dev libhiredis-dev libcurl4-openssl-dev

#Determine if we Need RapidJSON
if [ ! -d /usr/local/include/rapidjson ]; then

  printf "Cloning RapidJSON"

  mkdir $PRE/rapidjson

  #Get the RapidJSON Dependency
  git clone https://github.com/miloyip/rapidjson.git $PRE/rapidjson

  #Move the RapidJSON header files to the include path
  sudo cp -r $PRE/rapidjson/include/rapidjson/ /usr/local/include

fi

#Determine if we Need XRedis
if [ ! -d /usr/local/include/xredis ]; then

  printf "Building XRedis"

  mkdir $PRE/xredis
  git clone https://github.com/AO-StreetArt/xredis.git $PRE/xredis

  cd $PRE/xredis && make && sudo make install

fi

if [ ! -f /usr/local/include/zmq.h ]; then

  printf "Getting ZMQ"

  #Get the ZMQ Dependencies
  wget https://github.com/zeromq/zeromq4-1/releases/download/v4.1.4/zeromq-4.1.4.tar.gz

  #Build & Install ZMQ

  #Unzip the ZMQ Directories
  tar -xvzf zeromq-4.1.4.tar.gz

  printf "Building ZMQ"

  #Configure, make, install
  cd ./zeromq-4.1.4 && ./configure --without-libsodium && make && sudo make install

fi

#Run ldconfig to ensure that all built libraries are on the linker path
sudo ldconfig

if [ ! -f /usr/local/include/zmq.hpp ]; then

  printf "Cloning ZMQ C++ Bindings"

  #Get the ZMQ C++ Bindings
  git clone https://github.com/zeromq/cppzmq.git

  #Get ZMQ C++ Header files into include path
  sudo cp ./cppzmq/zmq.hpp /usr/local/include
  sudo cp ./cppzmq/zmq_addon.hpp /usr/local/include

fi

if [ ! -f ./couchbase-release-1.0-2-amd64.deb ]; then

  printf "Pulling Down Repositories for Couchbase Client"

  #Get the Couchbase dependecies
  wget http://packages.couchbase.com/releases/couchbase-release/couchbase-release-1.0-2-amd64.deb

fi

printf "Building Couchbase Client"

sudo dpkg -i ./couchbase-release-1.0-2-amd64.deb

printf "Update cache and install final dependencies through apt-get"

#Update the apt-get cache
sudo apt-get -y update

#Install the dependencies
sudo apt-get -y install libcouchbase-dev libcouchbase2-bin libprotobuf-dev protobuf-compiler liblog4cpp5-dev

printf "Finished installing dependencies"
