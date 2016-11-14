# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
#
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#
CC = g++
SLC = ar rcs
CFLAGS  = -g -Wall
STD = -std=c++11
NO_COUCHBASE = lib/cli.o lib/logging.o lib/http_admin.o lib/zmqio.o lib/redis_admin.o lib/consul_admin.o lib/logging_interface.o lib/uuid_admin.o\
 lib/service.o lib/http_server.o lib/properties_reader.o lib/mongo_admin.o
OBJS = $(NO_COUCHBASE) lib/couchbase_admin.o
TESTS_NOCR = cli_test consul_test logging_test http_test zmqio_test http_server_test properties_reader_test uuid_test mongo_test
TESTS_NOCOUCHBASE = $(TESTS_NOCR) redis_tests
TESTS = $(TESTS_NOCOUCHBASE) couchbase_test
TESTS_RHEL_NOCOUCHBASE = $(TESTS_NOCR) redis_tests_rhel
TESTS_RHEL = $(TESTS_NOCR) redis_tests_rhel couchbase_test
BENCHMARKS_NOREDIS = consul_benchmark logging_benchmark http_benchmark couchbase_benchmark
BENCHMARKS = $(BENCHMARKS_NOREDIS) redis_benchmark
BENCHMARKS_RHEL = $(BENCHMARKS_NOREDIS) redis_benchmark_rhel
INCL_NOCOUCHBASE = /usr/local/include/aossl /usr/local/include/aossl/factory_zmq.h /usr/local/include/aossl/factory_uuid.h /usr/local/include/aossl/factory_redis.h\
 /usr/local/include/aossl/factory_props.h /usr/local/include/aossl/factory_logging.h /usr/local/include/aossl/factory_http_server.h /usr/local/include/aossl/factory_http_client.h\
  /usr/local/include/aossl/factory_consul.h /usr/local/include/aossl/factory_cli.h /usr/local/include/aossl/cli.h /usr/local/include/aossl/consul_admin.h /usr/local/include/aossl/http_admin.h\
	 /usr/local/include/aossl/logging.h /usr/local/include/aossl/service.h /usr/local/include/aossl/uuid_admin.h /usr/local/include/aossl/redis_admin.h /usr/local/include/aossl/zmqio.h\
	  /usr/local/include/aossl/http_server.h /usr/local/include/aossl/properties_reader.h /usr/local/include/aossl/factory/properties_reader_interface.h\
		 /usr/local/include/aossl/factory/commandline_interface.h /usr/local/include/aossl/factory/consul_interface.h /usr/local/include/aossl/factory/db_admin.h\
		  /usr/local/include/aossl/factory/http_interface.h /usr/local/include/aossl/factory/logging_interface.h /usr/local/include/aossl/factory/uuid_interface.h\
			 /usr/local/include/aossl/factory/writeable.h /usr/local/include/aossl/factory/redis_interface.h /usr/local/include/aossl/factory/zmq_interface.h\
			  /usr/local/include/aossl/factory/http_server_interface.h /usr/local/include/aossl/factory/callbacks.h /usr/local/include/aossl/factory/interpreter.h\
				  /usr/local/include/aossl/mongo_admin.h\
				   /usr/local/include/aossl/factory/mongo_interface.h /usr/local/include/aossl/factory_mongo.h
INCL = $(INCL_NOCOUCHBASE) /usr/local/include/aossl/factory_couchbase.h /usr/local/include/aossl/couchbase_admin.h /usr/local/include/aossl/factory/couchbase_interface.h
BASE_DIR = /usr/local/include/aossl
INCL_DIR = /usr/local/include/aossl/factory
LIBS = -lpthread -llog4cpp
FULL_LIBS = -lpthread -llog4cpp -lzmq -luuid -lcurl -lcouchbase -levent -lmongoc-1.0 -lbson-1.0 `pkg-config --cflags --libs hiredis`
FULL_LIBS_RHEL = -lpthread -llog4cpp -lzmq -luuid -lcurl -lcouchbase -levent -lhiredis -lmongoc-1.0 -lbson-1.0
FACTORIES = lib/include/factory_cli.h lib/include/factory_consul.h lib/include/factory_couchbase.h lib/include/factory_http_client.h lib/include/factory_http_server.h lib/include/factory_logging.h\
	lib/include/factory_props.h lib/include/factory_redis.h lib/include/factory_uuid.h lib/include/factory_zmq.h lib/include/factory_mongo.h
INCL_DIRS = -I/usr/include/libbson-1.0 -I/usr/local/include/libmongoc-1.0 -I/usr/local/include/libbson-1.0
LINK_DIRS = -L/usr/local/lib

# typing 'make' will invoke the first target entry in the file
# (in this case the default target entry)
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention
#
default: libaossl.a

no-couchbase: libaossl-no-couchbase

# typing 'make test' will build the tests
test: $(TESTS)

test-no-couchbase: $(TESTS_NOCOUCHBASE)

# 'make rhel-test' will build the tests on RHEL/CentOS
rhel-test: $(TESTS_RHEL)

rhel-test-no-couchbase: $(TESTS_RHEL_NOCOUCHBASE)

# typing 'make benchmarks' will build the benchmarks
benchmarks: $(BENCHMARKS)

# typing 'make rhel-benchmarks' will build benchmarks on RHEL/CentOS
rhel-benchmarks: $(BENCHMARKS_RHEL)

# typing 'sudo make install' will install the libraries into system paths
install: /usr/local/lib/libaossl.a $(INCL)

# typing 'sudo make install' will install the libraries into system paths
install-no-couchbase: libaossl-no-couchbase $(INCL_NOCOUCHBASE)

# To start over from scratch, type 'make clean', then 'sudo make uninstall'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
uninstall: clean_install

clean: clean_local clean_tests clean_benchmarks

/usr/local/include/aossl:
	mkdir $(BASE_DIR)
	mkdir $(INCL_DIR)

/usr/local/include/aossl/factory_cli.h: lib/include/factory_cli.h
	cp $< $@

/usr/local/include/aossl/factory_consul.h: lib/include/factory_consul.h
	cp $< $@

/usr/local/include/aossl/factory_couchbase.h: lib/include/factory_couchbase.h
	cp $< $@

/usr/local/include/aossl/factory_http_client.h: lib/include/factory_http_client.h
	cp $< $@

/usr/local/include/aossl/factory_http_server.h: lib/include/factory_http_server.h
	cp $< $@

/usr/local/include/aossl/factory_logging.h: lib/include/factory_logging.h
	cp $< $@

/usr/local/include/aossl/factory_props.h: lib/include/factory_props.h
	cp $< $@

/usr/local/include/aossl/factory_redis.h: lib/include/factory_redis.h
	cp $< $@

/usr/local/include/aossl/factory_uuid.h: lib/include/factory_uuid.h
	cp $< $@

/usr/local/include/aossl/factory_zmq.h: lib/include/factory_zmq.h
	cp $< $@

/usr/local/include/aossl/cli.h: lib/include/cli.h
	cp $< $@

/usr/local/include/aossl/consul_admin.h: lib/include/consul_admin.h
	cp $< $@

/usr/local/include/aossl/couchbase_admin.h: lib/include/couchbase_admin.h
	cp $< $@

/usr/local/include/aossl/http_admin.h: lib/include/http_admin.h
	cp $< $@

/usr/local/include/aossl/logging.h: lib/include/logging.h
	cp $< $@

/usr/local/include/aossl/service.h: lib/include/service.h
	cp $< $@

/usr/local/include/aossl/uuid_admin.h: lib/include/uuid_admin.h
	cp $< $@

/usr/local/include/aossl/redis_admin.h: lib/include/redis_admin.h
	cp $< $@

/usr/local/include/aossl/zmqio.h: lib/include/zmqio.h
	cp $< $@

/usr/local/include/aossl/http_server.h: lib/include/http_server.h
	cp $< $@

/usr/local/include/aossl/properties_reader.h: lib/include/properties_reader.h
	cp $< $@

/usr/local/include/aossl/factory/properties_reader_interface.h: lib/include/factory/properties_reader_interface.h
	cp $< $@

/usr/local/include/aossl/factory/callbacks.h: lib/include/factory/callbacks.h
	cp $< $@

/usr/local/include/aossl/factory/commandline_interface.h: lib/include/factory/commandline_interface.h
	cp $< $@

/usr/local/include/aossl/factory/consul_interface.h: lib/include/factory/consul_interface.h
	cp $< $@

/usr/local/include/aossl/factory/couchbase_interface.h: lib/include/factory/couchbase_interface.h
	cp $< $@

/usr/local/include/aossl/factory/db_admin.h: lib/include/factory/db_admin.h
	cp $< $@

/usr/local/include/aossl/factory/http_interface.h: lib/include/factory/http_interface.h
	cp $< $@

/usr/local/include/aossl/factory/logging_interface.h: lib/include/factory/logging_interface.h
	cp $< $@

/usr/local/include/aossl/factory/uuid_interface.h: lib/include/factory/uuid_interface.h
	cp $< $@

/usr/local/include/aossl/factory/writeable.h: lib/include/factory/writeable.h
	cp $< $@

/usr/local/include/aossl/factory/redis_interface.h: lib/include/factory/redis_interface.h
	cp $< $@

/usr/local/include/aossl/factory/zmq_interface.h: lib/include/factory/zmq_interface.h
	cp $< $@

/usr/local/include/aossl/factory/http_server_interface.h: lib/include/factory/http_server_interface.h
	cp $< $@

/usr/local/include/aossl/factory/interpreter.h: lib/include/factory/interpreter.h
	cp $< $@

/usr/local/include/aossl/mongo_admin.h: lib/include/mongo_admin.h
	cp $< $@

/usr/local/include/aossl/factory/mongo_interface.h: lib/include/factory/mongo_interface.h
	cp $< $@

/usr/local/include/aossl/factory_mongo.h: lib/include/factory_mongo.h
	cp $< $@

/usr/local/lib/libaossl.a: libaossl.a
	cp $< $@

# Generate Benchmarks
consul_benchmark: lib/consul_benchmark.o lib/logging.o lib/consul_admin.o lib/http_admin.o lib/logging_interface.o lib/service.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -luuid -lcurl $(STD)

lib/consul_benchmark.o: lib/consul_benchmark.cpp lib/include/factory/consul_interface.h lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/consul_benchmark.cpp $(STD)

couchbase_benchmark: lib/couchbase_benchmark.o lib/logging.o lib/couchbase_admin.o lib/logging_interface.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -luuid -lcouchbase $(STD)

lib/couchbase_benchmark.o: lib/couchbase_benchmark.cpp lib/include/factory/couchbase_interface.h lib/include/factory/db_admin.h lib/include/factory/writeable.h lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/couchbase_benchmark.cpp $(STD)

http_benchmark: lib/http_benchmark.o lib/logging.o lib/http_admin.o lib/logging_interface.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -luuid -lcurl $(STD)

lib/http_benchmark.o: lib/http_benchmark.cpp lib/include/factory/http_interface.h lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/http_benchmark.cpp $(STD)

logging_benchmark: lib/logging_benchmark.o lib/logging.o lib/logging_interface.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD)

lib/logging_benchmark.o: lib/logging_benchmark.cpp lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/logging_benchmark.cpp $(STD)

redis_benchmark: lib/redis_benchmark.o lib/redis_admin.o lib/logging.o lib/logging_interface.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -luuid `pkg-config --cflags --libs hiredis` $(STD)

redis_benchmark_rhel: lib/redis_benchmark.o lib/redis_admin.o lib/logging.o lib/logging_interface.o
	$(CC) $(CFLAGS) -o redis_benchmark $^ $(LIBS) -luuid -lhiredis $(STD)

lib/redis_benchmark.o: lib/redis_benchmark.cpp lib/include/factory/redis_interface.h lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/redis_benchmark.cpp $(STD)

uuid_benchmark: lib/uuid_benchmark.o lib/logging.o lib/uuid_admin.o lib/logging_interface.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -luuid $(STD)

lib/uuid_benchmark.o: lib/uuid_benchmark.cpp lib/include/factory/uuid_interface.h lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/uuid_benchmark.cpp $(STD)

zmqio_benchmark: lib/zmqio_benchmark.o lib/logging.o lib/zmqio.o lib/logging_interface.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -lzmq $(STD)

lib/zmqio_benchmark.o: lib/zmqio_benchmark.cpp lib/include/factory/zmq_interface.h lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/zmqio_benchmark.cpp $(STD)

# Create the executable http_server_test
http_server_test: lib/http_server_test.o lib/http_server.o
	$(CC) $(CFLAGS) -o $@ $^ -levent $(STD)

lib/http_server_test.o: lib/http_server_test.cpp lib/include/http_server.h
	$(CC) $(CFLAGS) -o $@ -c lib/http_server_test.cpp $(STD)

# Create the executable file mongo_test
mongo_test: lib/mongo_test.o lib/mongo_admin.o
	$(CC) $(CFLAGS) -o $@ $^ $(STD) -lmongoc-1.0 -lbson-1.0 $(INCL_DIRS) $(LINK_DIRS)

lib/mongo_test.o: lib/mongo_test.cpp
	$(CC) $(CFLAGS) -o $@ -c lib/mongo_test.cpp $(STD) $(INCL_DIRS)

# Create the executable file cli_test
cli_test: lib/cli_test.o lib/cli.o
	$(CC) $(CFLAGS) -o $@ $^ $(STD)

# Create the object file cli_test.o
lib/cli_test.o: lib/cli_test.cpp lib/include/cli.h
	$(CC) $(CFLAGS) -o $@ -c lib/cli_test.cpp $(STD)

# Create the executable file consul_test
consul_test: lib/consul_test.o lib/consul_admin.o lib/http_admin.o lib/service.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -lcurl $(STD)

# Create the object file consul_test.o
lib/consul_test.o: lib/consul_test.cpp lib/include/consul_admin.h lib/include/http_admin.h lib/include/service.h
	$(CC) $(CFLAGS) -o $@ -c lib/consul_test.cpp $(STD)

# Create the executable file couchbase_test
couchbase_test: lib/couchbase_admin.o lib/couchbase_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -lcouchbase $(STD)

# Create the object file couchbase_test.o
lib/couchbase_test.o: lib/couchbase_test.cpp lib/include/couchbase_admin.h lib/include/logging.h lib/include/factory/db_admin.h lib/include/factory/writeable.h lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/couchbase_test.cpp $(STD)

# Create the executable file http_test
http_test: lib/http_admin.o lib/http_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -lcurl $(STD)

# Create the object file http_test.o
lib/http_test.o: lib/http_test.cpp lib/include/http_admin.h lib/include/logging.h lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/http_test.cpp $(STD)

# Create the executable file logging_test
logging_test: lib/logging.o lib/logging_test.o lib/logging_interface.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD)

# Create the object file logging_test.o
lib/logging_test.o: lib/logging_test.cpp lib/include/logging.h lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/logging_test.cpp $(STD)

# Create the executable file redis_tests
redis_tests: lib/redis_admin.o lib/redis_tests.o
	$(CC) $(CFLAGS) -o $@ $^ `pkg-config --cflags --libs hiredis` $(STD)

# Create the executable file redis_tests
redis_tests_rhel: lib/redis_admin.o lib/redis_tests.o
	$(CC) $(CFLAGS) -o redis_tests $^ -lhiredis $(STD)

# Create the object file redis_tests.o
lib/redis_tests.o: lib/redis_tests.cpp lib/include/redis_admin.h
	$(CC) $(CFLAGS) -o $@ -c lib/redis_tests.cpp $(STD)

# Create the executable file uuid_test
uuid_test: lib/uuid_admin.o lib/uuid_test.o
	$(CC) $(CFLAGS) -o $@ $^ -luuid $(STD)

# Create the object file uuid_test.o
lib/uuid_test.o: lib/uuid_test.cpp lib/include/uuid_admin.h
	$(CC) $(CFLAGS) -o $@ -c lib/uuid_test.cpp $(STD)

# Create the executable file zmqio_test
zmqio_test: lib/logging.o lib/zmqio.o lib/zmqio_test.o lib/logging_interface.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -lzmq $(STD)

# Create the object file zmqio_test.o
lib/zmqio_test.o: lib/zmqio_test.cpp lib/include/zmqio.h lib/include/logging.h lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/zmqio_test.cpp $(STD)

# Create the executable file properties_reader_test
properties_reader_test: lib/properties_reader_test.o lib/properties_reader.o
	$(CC) $(CFLAGS) -o $@ $^ $(STD)

# Create the object file properties_reader_test.o
lib/properties_reader_test.o: lib/properties_reader_test.cpp lib/properties_reader.cpp lib/include/properties_reader.h
	$(CC) $(CFLAGS) -o $@ -c lib/properties_reader_test.cpp $(STD)

factory_test: lib/factory_test.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(FULL_LIBS) $(STD)

factory_test_rhel: lib/factory_test.o $(OBJS)
	$(CC) $(CFLAGS) -o factory_test $^ $(FULL_LIBS_RHEL) $(STD)

lib/factory_test.o: lib/factory_test.cpp $(FACTORIES)
	$(CC) $(CFLAGS) -o $@ -c lib/factory_test.cpp $(STD)

# To create the static library we need the object files
# Using $@ gives us the current target
#
libaossl.a:  $(OBJS)
	$(SLC) $@ $(OBJS)

libaossl-no-couchbase: $(NO_COUCHBASE)
	$(SLC) libaossl.a $(NO_COUCHBASE)

# Create the object file mongo_admin.o
lib/mongo_admin.o: lib/mongo_admin.cpp lib/include/mongo_admin.h lib/include/factory/mongo_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/mongo_admin.cpp $(STD) $(INCL_DIRS)

# Create the object file consul_admin.o
lib/consul_admin.o:  lib/consul_admin.cpp lib/include/factory/logging_interface.h lib/include/http_admin.h lib/include/service.h lib/include/factory/consul_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/consul_admin.cpp $(STD)

lib/service.o: lib/service.cpp lib/include/service.h lib/include/factory/consul_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/service.cpp $(STD)

# Create the object file http_server.o
lib/http_server.o: lib/http_server.cpp lib/include/http_server.h lib/include/factory/http_server_interface.h lib/include/factory/callbacks.h
	$(CC) $(CFLAGS) -o $@ -c lib/http_server.cpp $(STD)

# Create the object file redis_admin.o
lib/redis_admin.o:  lib/redis_admin.cpp
	$(CC) $(CFLAGS) -o $@ -c lib/redis_admin.cpp $(STD)

# Create the object file couchbase_admin.o
lib/couchbase_admin.o:  lib/couchbase_admin.cpp lib/include/couchbase_admin.h lib/include/logging.h lib/include/factory/db_admin.h lib/include/factory/writeable.h
	$(CC) $(CFLAGS) -o $@ -c lib/couchbase_admin.cpp $(STD)

# Create the object file zmqio.o
lib/zmqio.o:  lib/zmqio.cpp lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/zmqio.cpp $(STD)

# Create the object file http_admin.o
lib/http_admin.o:  lib/http_admin.cpp lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/http_admin.cpp $(STD)

# Create the object file uuid_admin.o
lib/uuid_admin.o:  lib/uuid_admin.cpp lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/uuid_admin.cpp $(STD)

# Create the object file logging.o
lib/logging.o:  lib/logging.cpp lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/logging.cpp $(STD)

# Create the object file cli.o
lib/cli.o:  lib/cli.cpp lib/include/cli.h lib/include/factory/commandline_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/cli.cpp $(STD)

# Create the object file properties_reader.o
lib/properties_reader.o: lib/properties_reader.cpp lib/include/properties_reader.h lib/include/factory/properties_reader_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/properties_reader.cpp $(STD)

lib/logging_interface.o: lib/logging_interface.cpp lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/logging_interface.cpp $(STD)

# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean: clean_local clean_tests clean_benchmarks

uninstall: clean_install

clean_local:
	$(RM) libaossl.a lib/*.o *~

clean_tests:
	$(RM) *_test *_tests

clean_benchmarks:
	$(RM) *_benchmark

clean_install:
	$(RM) -r /usr/local/include/aossl
	$(RM) /usr/local/lib/libaossl.a
