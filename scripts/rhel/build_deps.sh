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
yum -y install build-essential libtool pkg-config autoconf automake cmake make git wget gcc gcc-c++ epel-release libuuid-devel libcurl-devel
yum repolist

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
