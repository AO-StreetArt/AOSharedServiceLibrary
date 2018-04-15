#!/bin/bash
set -e
#This script will attempt to build AOSSL dependencies

#Based on CentOS 7

printf "Creating Dependency Folder"
PRE=./downloads
mkdir $PRE

printf "Calling yum update"

#Update the Server
yum -y update

#Build the dependencies and place them in the correct places

printf "Addressing pre-build requirements"

#Ensure that specific build requirements are satisfied
yum -y install build-essential libtool pkg-config autoconf automake cmake make git wget gcc gcc-c++ epel-release
yum repolist

#Determine if we need the neo4j-client library
printf "Building libneo4j"

mkdir $PRE/neo
wget https://github.com/cleishm/libneo4j-client/releases/download/v2.1.3/libneo4j-client-2.1.3.tar.gz -P ./$PRE

tar -zxvf $PRE/libneo4j-client-2.1.3.tar.gz -C $PRE/neo
cd $PRE/neo/libneo4j-client-2.1.3 && ./configure --disable-tools --without-tls && make clean check && make install
cd ../../

printf "Building Mongo C Driver"
wget https://github.com/mongodb/mongo-c-driver/releases/download/1.9.4/mongo-c-driver-1.9.4.tar.gz
tar xzf mongo-c-driver-1.9.4.tar.gz
cd mongo-c-driver-1.9.4 && ./configure --disable-automatic-init-and-cleanup --with-libbson=bundled && make && make install

#Determine if we Need Redis Client
if [ ! -d /usr/local/include/hiredis ]; then

  printf "Building libhiredis"

  mkdir $PRE/hiredis
  git clone https://github.com/redis/hiredis.git ./$PRE/hiredis

  cd $PRE/hiredis && make && make install
  cd ../../

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
  cd ./zeromq-4.1.4 && ./configure --without-libsodium && make && make install
  cd ../

  #Add /usr/local/lib to linker path
  sh -c 'echo "/usr/local/lib" >> /etc/ld.so.conf.d/usrlib.conf'

fi

if [ ! -f /usr/local/include/zmq.hpp ]; then

  printf "Cloning ZMQ C++ Bindings"

  #Get the ZMQ C++ Bindings
  git clone https://github.com/zeromq/cppzmq.git

  #Get ZMQ C++ Header files into include path
  cp ./cppzmq/zmq.hpp /usr/local/include
  cp ./cppzmq/zmq_addon.hpp /usr/local/include

fi

printf "Building Libevent"
wget https://github.com/libevent/libevent/releases/download/release-2.1.8-stable/libevent-2.1.8-stable.tar.gz
tar -xvzf libevent-2.1.8-stable.tar.gz
cd libevent-2.1.8-stable && ./configure && make && make install

printf "Building Hayai, optional, for benchmarks"

#Install hayai, for compiling benchmarks
mkdir $PRE/hayai
git clone https://github.com/nickbruun/hayai.git ./$PRE/hayai
cd ./$PRE/hayai && cmake . && make && make install
cd ../../

printf "Update cache and install final dependencies through yum"
#Update the yum cache
yum -y update

#Install the dependencies
yum -y install libuuid-devel libcurl-devel log4cpp-devel log4cpp

#Run ldconfig to ensure that all built libraries are on the linker path
ldconfig

printf "Finished installing dependencies"
