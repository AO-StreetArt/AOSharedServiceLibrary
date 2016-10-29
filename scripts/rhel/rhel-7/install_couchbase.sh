#!/bin/bash

#Get and install couchbase server
wget http://packages.couchbase.com/releases/4.1.1/couchbase-server-community-4.1.1-centos7.x86_64.rpm
sudo yum -y install couchbase-server-community-4.1.1-centos7.x86_64.rpm

#Configure the couchbase server
cd /opt/couchbase
./bin/install/reloc.sh `pwd`
./bin/couchbase-server -- -noinput -detached
sleep 20
./bin/couchbase-cli cluster-init -c 127.0.0.1:8091  --cluster-init-username=Administrator --cluster-init-password=password --cluster-init-port=8091 --cluster-init-ramsize=2048
./bin/couchbase-cli bucket-create -c 127.0.0.1:8091 --bucket=default --bucket-type=couchbase --bucket-port=11211 --bucket-ramsize=408  --bucket-replica=1 -u Administrator -p password
./bin/couchbase-cli bucket-create -c 127.0.0.1:8091 --bucket=test --bucket-type=couchbase --bucket-port=11211 --bucket-ramsize=408  --bucket-replica=1 -u Administrator -p password
./bin/couchbase-cli bucket-create -c 127.0.0.1:8091 --bucket=test2 --bucket-type=couchbase --bucket-port=11211 --bucket-ramsize=408  --bucket-replica=1 -u Administrator -p password
