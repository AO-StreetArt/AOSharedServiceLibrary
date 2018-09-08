#!/bin/bash
set -e
#This script will attempt to build AOSSL dependencies

#Based on Ubuntu 18.04 LTS
INSTALL_OPT="-none"
if [ "$#" -eq 1 ]; then
  INSTALL_OPT=$1
fi

printf "Creating Dependency Folder\n"
PRE=./downloads
mkdir $PRE

printf "apt-get setup\n"
apt-get -y update

#Install the basic tools we need
apt-get install -y zlib1g-dev apt-utils debconf-utils iputils-ping wget curl mc htop ssh software-properties-common build-essential libtool pkg-config automake cmake uuid-dev libcurl4-openssl-dev git libsnappy-dev liblog4cpp5-dev libssl-dev openssl

#Build POCO
printf "Installing Poco\n"

if [ "$INSTALL_OPT" != "-no-poco" ]; then
  wget https://pocoproject.org/releases/poco-1.9.0/poco-1.9.0-all.tar.gz
  tar -xvzf poco-1.9.0-all.tar.gz
  cd poco-1.9.0-all && ./configure --omit=Data/ODBC,Data/MySQL && make -s && make -s install
  cd ../
fi

printf "Installing Rapidjson\n"
git clone https://github.com/miloyip/rapidjson.git
cp -r rapidjson/include/rapidjson/ /usr/local/include

printf "Building Hayai, optional, for benchmarks\n"
git clone https://github.com/nickbruun/hayai.git
cd hayai && cmake . && make && make install

#Run ldconfig to ensure that all built libraries are on the linker path
ldconfig

printf "Finished installing dependencies"
