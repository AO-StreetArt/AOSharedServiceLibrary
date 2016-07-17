#!/bin/bash

opt=$1

if [ -n "$opt" ]
then

  if [ $opt == "g" ]
  then

    #We start by compiling the logging module:

    g++ -g -c -o lib/logging.o lib/logging.cpp -std=c++11

	#Next, we compile the command line interpreter

	g++ -g -c -o lib/cli.o lib/cli.cpp -std=c++11

    #We then compile the http administrator

    g++ -g -c -o lib/http_admin.o lib/http_admin.cpp -std=c++11

    #We compile the Inbound & Outbound ZMQ Socket managers

    g++ -g -c -o lib/zmqio.o lib/zmqio.cpp -std=c++11

    #We then build the couchbase admin:

    g++ -g -c -o lib/couchbase_admin.o lib/couchbase_admin.cpp -std=c++11

    #Now, we build the Redis Admin

    g++ -g -c -o lib/xredis_admin.o lib/xredis_admin.cpp -std=c++11

    #Next, we build the Consul Admin

    g++ -g -c -o lib/consul_admin.o lib/consul_admin.cpp -std=c++11

    #Finally, We build the UUID Admin

    g++ -g -c -o lib/uuid_admin.o lib/uuid_admin.cpp -std=c++11

    echo "Debugger Lines Set"

  fi

else

  #We start by compiling the logging module:

  g++ -c -o lib/logging.o lib/logging.cpp -std=c++11

  #Next, we compile the command line interpreter

  g++ -c -o lib/cli.o lib/cli.cpp -std=c++11

  #We then compile the http administrator

  g++ -c -o lib/http_admin.o lib/http_admin.cpp -std=c++11

  #We compile the Inbound & Outbound ZMQ Socket managers

  g++ -c -o lib/zmqio.o lib/zmqio.cpp -std=c++11

  #We then build the couchbase admin:

  g++ -c -o lib/couchbase_admin.o lib/couchbase_admin.cpp -std=c++11

  #Now, we build the Redis Admin

  g++ -c -o lib/xredis_admin.o lib/xredis_admin.cpp -std=c++11

  #Next, we build the Consul Admin

  g++ -c -o lib/consul_admin.o lib/consul_admin.cpp -std=c++11

  #Finally, We build the UUID Admin

  g++ -c -o lib/uuid_admin.o lib/uuid_admin.cpp -std=c++11

  echo "Build Object Libraries Completed"

fi

#Build the static library

ar rcs libaossl.a lib/logging.o lib/http_admin.o lib/zmqio.o lib/couchbase_admin.o lib/xredis_admin.o lib/consul_admin.o lib/uuid_admin.o lib/cli.o
