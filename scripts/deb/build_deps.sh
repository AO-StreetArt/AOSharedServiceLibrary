#!/bin/bash
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
sudo apt-get -y install build-essential libtool pkg-config autoconf automake uuid-dev libhiredis-dev libcurl4-openssl-dev libevent-dev

#Determine if we need the neo4j-client library
printf "Building libneo4j"

mkdir $PRE/neo
git clone https://github.com/cleishm/libneo4j-client.git ./$PRE/neo

cd ./$PRE/neo && ./autogen.sh && ./configure --disable-tools && make clean check && sudo make install

#Determine if we need the BSON Library
if [ ! -d /usr/local/include/libbson-1.0 ]; then

  printf "Building libbson"

  mkdir $PRE/bson
  git clone git://github.com/mongodb/libbson.git ./$PRE/bson

  cd ./$PRE/bson && ./autogen.sh && make && sudo make install

fi

#Determine if we need Mongo Client
if [ ! -d /usr/local/include/libmongoc-1.0 ]; then

  printf "Building libmongoc"

  mkdir $PRE/mongo
  git clone https://github.com/mongodb/mongo-c-driver.git ./$PRE/mongo

  cd ./$PRE/mongo && ./autogen.sh --with-libbson=bundled && make && sudo make install

fi

#Determine if we Need XRedis
if [ ! -d /usr/local/include/hiredis ]; then

  printf "Building HiRedis"

  mkdir $PRE/hiredis
  git clone https://github.com/redis/hiredis.git $PRE/hiredis

  cd $PRE/hiredis && make && sudo make install

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

printf "Building Hayai, optional, for benchmarks"

#Install hayai, for compiling benchmarks
sudo apt-add-repository -y ppa:bruun/hayai
sudo apt-get update -y
sudo apt-get install -y libhayai-dev

printf "Building Couchbase Client"

sudo dpkg -i ./couchbase-release-1.0-2-amd64.deb

printf "Update cache and install final dependencies through apt-get"

#Update the apt-get cache
sudo apt-get -y update

#Install the dependencies
sudo apt-get -y install libcouchbase-dev libcouchbase2-bin liblog4cpp5-dev

printf "Finished installing dependencies"
