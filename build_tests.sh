#!/bin/bash
exec 3>&1 4>&2
trap 'exec 2>&4 1>&3' 0 1 2 3
exec 1>build_tests.log 2>&1

opt=$1

if [ -n "$opt" ]
then

  if [ $opt == "g" ]
  then

    #We build the logging tests
    g++ -g -c -o lib/logging_test.o lib/logging_test.cpp -std=c++11
    g++ -g -o logging_test lib/logging.o lib/logging_test.o -lpthread -llog4cpp -std=c++11

    #We build the UUID Tests
    g++ -g -c -o lib/uuid_test.o lib/uuid_test.cpp -std=c++11
    g++ -g -o uuid_test lib/logging.o lib/uuid_admin.o lib/uuid_test.o -lpthread -llog4cpp -luuid -std=c++11

    #We have the HTTP Tests
    g++ -g -c -o lib/http_test.o lib/http_test.cpp -std=c++11
    g++ -g -o http_test lib/logging.o lib/http_admin.o lib/http_test.o -lpthread -llog4cpp -lcurl -std=c++11

    #We can build the couchbase tests:
    g++ -g -c -o lib/couchbase_test.o lib/couchbase_test.cpp -std=c++11
    g++ -g -o cb_test lib/logging.o lib/couchbase_admin.o lib/couchbase_test.o -lpthread -llog4cpp -lcouchbase -std=c++11

    #Next, the Redis tests:
    g++ -g -c -o lib/redis_test.o lib/redis_test.cpp -std=c++11
    g++ -g -o redis_test lib/logging.o lib/xredis_admin.o lib/redis_test.o -lpthread -llog4cpp -lxredis `pkg-config --cflags --libs hiredis` -std=c++11

    #Next, the Consul tests:
    g++ -g -c -o lib/consul_test.o lib/consul_test.cpp -std=c++11
    g++ -g -o consul_test lib/logging.o lib/http_admin.o lib/consul_admin.o lib/consul_test.o -lpthread -llog4cpp -lcurl -std=c++11

    #Finally, we build the ZMQ Tests
    g++ -g -c -o lib/zmqio_test.o lib/zmqio_test.cpp
    g++ -g -o zmqio_test lib/logging.o lib/zmqio.o lib/zmqio_test.o -lpthread -llog4cpp -lzmq -std=c++11

    echo "Debugger Lines Set"

  fi

else

  #We build the logging tests
  g++ -c -o lib/logging_test.o lib/logging_test.cpp -std=c++11
  g++ -o logging_test lib/logging.o lib/logging_test.o -lpthread -llog4cpp -std=c++11

  #We build the UUID Tests
  g++ -c -o lib/uuid_test.o lib/uuid_test.cpp -std=c++11
  g++ -o uuid_test lib/logging.o lib/uuid_admin.o lib/uuid_test.o -lpthread -llog4cpp -luuid -std=c++11

  #We have the HTTP Tests
  g++ -c -o lib/http_test.o lib/http_test.cpp -std=c++11
  g++ -o http_test lib/logging.o lib/http_admin.o lib/http_test.o -lpthread -llog4cpp -lcurl -std=c++11

  #We can build the couchbase tests:
  g++ -c -o lib/couchbase_test.o lib/couchbase_test.cpp -std=c++11
  g++ -o cb_test lib/logging.o lib/couchbase_admin.o lib/couchbase_test.o -lpthread -llog4cpp -lcouchbase -std=c++11

  #Next, the Redis tests:
  g++ -c -o lib/redis_test.o lib/redis_test.cpp -std=c++11
  g++ -o redis_test lib/logging.o lib/xredis_admin.o lib/redis_test.o -lpthread -llog4cpp -lxredis `pkg-config --cflags --libs hiredis` -std=c++11

  #Next, the Consul tests:
  g++ -c -o lib/consul_test.o lib/consul_test.cpp -std=c++11
  g++ -o consul_test lib/logging.o lib/http_admin.o lib/consul_admin.o lib/consul_test.o -lpthread -llog4cpp -lcurl -std=c++11

  #Finally, we build the ZMQ Tests
  g++ -c -o lib/zmqio_test.o lib/zmqio_test.cpp
  g++ -o zmqio_test lib/logging.o lib/zmqio.o lib/zmqio_test.o -lpthread -llog4cpp -lzmq -std=c++11

fi
