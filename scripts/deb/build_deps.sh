#!/bin/bash
set -e
#This script will attempt to build AOSSL dependencies

#Based on Ubuntu 14.04 LTS

printf "Creating Dependency Folder"
PRE=./downloads
mkdir $PRE

printf "apt-get setup"

#Install latest version of autoconf
#Fix for Travis CI Builds which don't have latest version installed
add-apt-repository ppa:dns/gnu -y
apt-get -y -q update
apt-get install -y --only-upgrade autoconf

#Install the basic tools we need
apt-get install -y apt-utils debconf-utils iputils-ping wget curl mc htop ssh software-properties-common

#Add libneo4j repository
add-apt-repository ppa:cleishm/neo4j -y
apt-get -y -q update

printf "Starting with apt-get dependencies"
apt-get -y -q install build-essential libtool pkg-config automake cmake uuid-dev libhiredis-dev libcurl4-openssl-dev libevent-dev git libsnappy-dev liblog4cpp5-dev libssl-dev openssl neo4j-client libneo4j-client-dev

printf "Building Mongo C Driver"
wget https://github.com/mongodb/mongo-c-driver/releases/download/1.6.3/mongo-c-driver-1.6.3.tar.gz
tar xzf mongo-c-driver-1.6.3.tar.gz
cd mongo-c-driver-1.6.3 && ./configure --disable-automatic-init-and-cleanup --with-libbson=bundled && make && make install

if [ ! -f /usr/local/include/zmq.h ]; then

  printf "Getting ZMQ"

  #Get the ZMQ Dependencies
  wget https://github.com/zeromq/zeromq4-1/releases/download/v4.1.4/zeromq-4.1.4.tar.gz

  #Build & Install ZMQ

  #Unzip the ZMQ Directories
  tar -xvzf zeromq-4.1.4.tar.gz

  printf "Building ZMQ"

  #Configure, make, install
  cd ./zeromq-4.1.4 && ./configure --without-libsodium && make && make install
  cd ../

fi

if [ ! -f /usr/local/include/zmq.hpp ]; then

  printf "Cloning ZMQ C++ Bindings"

  #Get the ZMQ C++ Bindings
  git clone https://github.com/zeromq/cppzmq.git

  #Get ZMQ C++ Header files into include path
  cp ./cppzmq/zmq.hpp /usr/local/include
  cp ./cppzmq/zmq_addon.hpp /usr/local/include

fi

printf "Building Hayai, optional, for benchmarks"
git clone https://github.com/nickbruun/hayai.git
cd hayai && cmake . && make && make install

#Run ldconfig to ensure that all built libraries are on the linker path
ldconfig

printf "Finished installing dependencies"
