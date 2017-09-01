################################################################

# Dockerfile to build AOSSL Container Images
# Based on Ubuntu-ssh

################################################################

#Based on Ubuntu 16.04
FROM ubuntu:16.04

#Set the Maintainer
MAINTAINER Alex Barry

#Set up front end
ENV DEBIAN_FRONTEND noninteractive

#Setup basic environment tools
RUN apt-get update
RUN	apt-get install -y apt-utils debconf-utils iputils-ping wget curl mc htop ssh software-properties-common
RUN	apt-get clean

#Setup necessary components for building the library
RUN add-apt-repository -y ppa:cleishm/neo4j
RUN apt-get update
RUN apt-get install -y build-essential libtool pkg-config autoconf automake cmake uuid-dev libhiredis-dev libcurl4-openssl-dev libevent-dev git libsnappy-dev liblog4cpp5-dev neo4j-client libzmq-dev

#Get the Mongo Dependencies, we build from source as the version provided by apt-get uses deprecated functions
RUN wget https://github.com/mongodb/mongo-c-driver/releases/download/1.6.3/mongo-c-driver-1.6.3.tar.gz
RUN tar xzf mongo-c-driver-1.6.3.tar.gz
RUN cd mongo-c-driver-1.6.3 && ./configure --disable-automatic-init-and-cleanup --with-libbson=bundled && make && make install

#Get Hayai, for benchmarks
RUN git clone https://github.com/nickbruun/hayai.git
RUN cd hayai && cmake . && make && make install

#Run ldconfig to ensure that Hayai is on the linker path
RUN ldconfig

#Get the ZMQ C++ Bindings
RUN git clone https://github.com/zeromq/cppzmq.git

#Get ZMQ C++ Header files into include path
RUN cp cppzmq/zmq.hpp /usr/local/include
RUN cp cppzmq/zmq_addon.hpp /usr/local/include

#Expose some of the default ports
EXPOSE 22
EXPOSE 5555
EXPOSE 5556
EXPOSE 8091
EXPOSE 8092
EXPOSE 8093
EXPOSE 11210
EXPOSE 12345

#Pull the project source from github
RUN git clone https://github.com/AO-StreetArt/AOSharedServiceLibrary.git

#Install the library
RUN cd AOSharedServiceLibrary && make && make install

#Start up the SSH terminal so that we can connect & start the app
CMD tail -f /dev/null
