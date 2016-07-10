#!/bin/bash
exec 3>&1 4>&2
trap 'exec 2>&4 1>&3' 0 1 2 3
exec 1>build_project.log 2>&1

version_major=0
version_minor=1

opt=$1

if [ -n "$opt" ]
then

  if [ $opt == "g" ]
  then

    #We start by compiling the logging module:

    g++ -g -c -o lib/logging.o lib/logging.cpp -std=c++11

    #We then compile the http administrator

    g++ -g -c -o lib/http_admin.o lib/http_admin.cpp -std=c++11

    #We compile the Outbound ZMQ Socket

    g++ -g -c -o lib/zmqo.o lib/zmqo.cpp -std=c++11

    #We then build the couchbase admin:

    g++ -g -c -o lib/couchbase_admin.o lib/couchbase_admin.cpp -std=c++11

    #Now, we build the Redis Admin

    g++ -g -c -o lib/xredis_admin.o lib/xredis_admin.cpp -std=c++11

    #Finally, We build the UUID Admin

    g++ -g -c -o lib/uuid_admin.o lib/uuid_admin.cpp -std=c++11

    echo "Debugger Lines Set"

  fi

else

  #We start by compiling the logging module:

  g++ -c -o lib/logging.o lib/logging.cpp -std=c++11

  #We then compile the http administrator

  g++ -c -o lib/http_admin.o lib/http_admin.cpp -std=c++11

  #We compile the Outbound ZMQ Socket

  g++ -c -o lib/zmqo.o lib/zmqo.cpp -std=c++11

  #We then build the couchbase admin:

  g++ -c -o lib/couchbase_admin.o lib/couchbase_admin.cpp -std=c++11

  #Now, we build the Redis Admin

  g++ -c -o lib/xredis_admin.o lib/xredis_admin.cpp -std=c++11

  #Finally, We build the UUID Admin

  g++ -c -o lib/uuid_admin.o lib/uuid_admin.cpp -std=c++11

  echo "Build Object Libraries Completed"

fi

#Build the static library

ar rcs libaossl.a.$version_major.$version_minor lib/logging.o lib/http_admin.o lib/zmqo.o lib/couchbase_admin.o lib/xredis_admin.o lib/uuid_admin.o
