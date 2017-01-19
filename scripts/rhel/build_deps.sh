#!/bin/bash
#This script will attempt to build AOSSL dependencies

#Based on CentOS 7

printf "Creating Dependency Folder"
PRE=./downloads
mkdir $PRE

printf "Calling apt-get update"

#Update the Server
sudo yum -y update

#Build the dependencies and place them in the correct places

printf "Addressing pre-build requirements"

#Ensure that specific build requirements are satisfied
sudo yum -y install build-essential libtool pkg-config autoconf automake cmake make git wget gcc gcc-c++

#Determine if we need the neo4j-client library
printf "Building libneo4j"

mkdir $PRE/neo
git clone https://github.com/cleishm/libneo4j-client.git ./$PRE/neo

cd ./$PRE/neo && ./autogen.sh && ./configure --disable-tools && make clean check && sudo make install

#Determine if we need Mongo Client
if [ ! -d /usr/local/include/libmongoc-1.0 ]; then

  printf "Building libmongoc"

  mkdir $PRE/mongo
  git clone https://github.com/mongodb/mongo-c-driver.git ./$PRE/mongo

  cd ./$PRE/mongo && ./autogen.sh --with-libbson=bundled && make && sudo make install

fi

#Determine if we Need Redis Client
if [ ! -d /usr/local/include/hiredis ]; then

  printf "Building libhiredis"

  mkdir $PRE/hiredis
  git clone https://github.com/redis/hiredis.git ./$PRE/hiredis

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

  #Add /usr/local/lib to linker path
  sudo sh -c 'echo "/usr/local/lib" >> /etc/ld.so.conf.d/usrlib.conf'

fi

if [ ! -f /usr/local/include/zmq.hpp ]; then

  printf "Cloning ZMQ C++ Bindings"

  #Get the ZMQ C++ Bindings
  git clone https://github.com/zeromq/cppzmq.git

  #Get ZMQ C++ Header files into include path
  sudo cp ./cppzmq/zmq.hpp /usr/local/include
  sudo cp ./cppzmq/zmq_addon.hpp /usr/local/include

fi

if [ ! -f ./couchbase-release-1.0-2-amd64.deb ]; then

printf "Building Hayai, optional, for benchmarks"

#Install hayai, for compiling benchmarks
mkdir $PRE/hayai
git clone https://github.com/nickbruun/hayai.git ./$PRE/hayai
cd ./$PRE/hayai && cmake . && make && sudo make install

fi

printf "Update cache and install final dependencies through apt-get"

#Log4cpp install
mkdir $PRE/log4cpp
wget ftp://mirror.switch.ch/pool/4/mirror/epel/7/x86_64/l/log4cpp-devel-1.1.1-1.el7.x86_64.rpm
wget ftp://mirror.switch.ch/pool/4/mirror/epel/7/x86_64/l/log4cpp-1.1.1-1.el7.x86_64.rpm

sudo rpm -Uvh log4cpp-1.1.1-1.el7.x86_64.rpm
sudo rpm -Uvh log4cpp-devel-1.1.1-1.el7.x86_64.rpm

#Update the apt-get cache
sudo yum -y update

#Install the dependencies
sudo yum -y install libuuid-devel libcurl-devel libevent-devel

#Run ldconfig to ensure that all built libraries are on the linker path
sudo ldconfig

printf "Finished installing dependencies"
