#!/bin/bash

#Get and install couchbase server
wget http://packages.couchbase.com/releases/4.1.0/couchbase-server-community_4.1.0-ubuntu12.04_amd64.deb
sudo dpkg -i couchbase-server-community_4.1.0-ubuntu12.04_amd64.deb .

#Configure the couchbase server
cd /opt/couchbase
./bin/install/reloc.sh `pwd`
./bin/couchbase-server -- -noinput -detached
sleep 20
./bin/couchbase-cli cluster-init -c 127.0.0.1:8091  --cluster-init-username=Administrator --cluster-init-password=password --cluster-init-port=8091 --cluster-init-ramsize=1024
./bin/couchbase-cli bucket-create -c 127.0.0.1:8091 --bucket=default --bucket-type=couchbase --bucket-port=11211 --bucket-ramsize=512  --bucket-replica=1 -u Administrator -p password
