#!/bin/bash
set -e
#This script will attempt to build AOSSL dependencies

#Based on CentOS 7
INSTALL_OPT="-none"
if [ "$#" -eq 1 ]; then
  INSTALL_OPT=$1
fi

printf "Creating Dependency Folder"
PRE=./downloads
mkdir $PRE

printf "Calling yum update"

#Update the Server
yum -y update

#Build the dependencies and place them in the correct places

printf "Addressing pre-build requirements"

#Ensure that specific build requirements are satisfied
yum -y install build-essential libtool autoconf automake cmake make git wget gcc gcc-c++ epel-release libuuid-devel zlib-devel openssl openssl-devel
yum repolist

#Build POCO
if [ "$INSTALL_OPT" != "-no-poco" ]; then
  printf "Installing Poco\n"
  wget https://pocoproject.org/releases/poco-1.9.0/poco-1.9.0-all.tar.gz
  tar -xvzf poco-1.9.0-all.tar.gz
  cd poco-1.9.0-all && ./configure --omit=Data/ODBC,Data/MySQL && make -s && make -s install
  cd ../
fi

printf "Installing Rapidjson"
git clone https://github.com/miloyip/rapidjson.git
cp -r rapidjson/include/rapidjson/ /usr/local/include

printf "Building Hayai, optional, for benchmarks"

#Install hayai, for compiling benchmarks
mkdir $PRE/hayai
git clone https://github.com/nickbruun/hayai.git ./$PRE/hayai
cd ./$PRE/hayai && cmake . && make && make install
cd ../../

#Run ldconfig to ensure that all built libraries are on the linker path
ldconfig

printf "Finished installing dependencies"
