#!/bin/bash
set -e
#This script will attempt to build AOSSL dependencies

#Based on Ubuntu 14.04 LTS

printf "Creating Dependency Folder\n"
PRE=./downloads
mkdir $PRE

printf "apt-get setup\n"
apt-get -y update

#Install the basic tools we need
apt-get install -y apt-utils debconf-utils iputils-ping wget curl mc htop ssh software-properties-common build-essential libtool pkg-config automake cmake uuid-dev libcurl4-openssl-dev git libsnappy-dev liblog4cpp5-dev libssl-dev openssl

printf "Installing Rapidjson"
git clone https://github.com/miloyip/rapidjson.git
cp -r rapidjson/include/rapidjson/ /usr/local/include

printf "Building Hayai, optional, for benchmarks"
git clone https://github.com/nickbruun/hayai.git
cd hayai && cmake . && make && make install

#Run ldconfig to ensure that all built libraries are on the linker path
ldconfig

printf "Finished installing dependencies"
