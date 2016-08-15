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
OBJS = lib/cli.o lib/logging.o lib/http_admin.o lib/zmqio.o lib/couchbase_admin.o lib/xredis_admin.o lib/consul_admin.o lib/factory.o lib/logging_interface.o lib/uuid_admin.o lib/service.o
INCL = /usr/local/include/aossl /usr/local/include/aossl/factory.h /usr/local/include/aossl/cli.h /usr/local/include/aossl/consul_admin.h /usr/local/include/aossl/couchbase_admin.h /usr/local/include/aossl/http_admin.h /usr/local/include/aossl/logging.h /usr/local/include/aossl/service.h /usr/local/include/aossl/uuid_admin.h /usr/local/include/aossl/xredis_admin.h /usr/local/include/aossl/zmqio.h /usr/local/include/aossl/factory/commandline_interface.h /usr/local/include/aossl/factory/consul_interface.h /usr/local/include/aossl/factory/couchbase_interface.h /usr/local/include/aossl/factory/db_admin.h /usr/local/include/aossl/factory/http_interface.h /usr/local/include/aossl/factory/logging_interface.h /usr/local/include/aossl/factory/uuid_interface.h /usr/local/include/aossl/factory/writeable.h /usr/local/include/aossl/factory/redis_interface.h /usr/local/include/aossl/factory/zmq_interface.h
BASE_DIR = /usr/local/include/aossl
INCL_DIR = /usr/local/include/aossl/factory
TESTS = cli_test consul_test couchbase_test http_test logging_test redis_test uuid_test zmqio_test factory_test
BENCHMARKS = consul_benchmark couchbase_benchmark http_benchmark logging_benchmark redis_benchmark uuid_benchmark zmqio_benchmark
LIBS = -lpthread -llog4cpp
FULL_LIBS = -lpthread -llog4cpp -lzmq -luuid -lxredis -lcurl -lcouchbase `pkg-config --cflags --libs hiredis`

# typing 'make' will invoke the first target entry in the file
# (in this case the default target entry)
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention
#
default: libaossl.a

# typing 'make test' will build the tests
test: $(TESTS)

# typing 'make benchmarks' will build the benchmarks
benchmarks: $(BENCHMARKS)

# typing 'sudo make install' will install the libraries into system paths
install: /usr/local/lib/libaossl.a $(INCL)

# To start over from scratch, type 'make clean', then 'sudo make uninstall'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
uninstall: clean_install

clean: clean_local clean_tests clean_benchmarks

/usr/local/include/aossl:
	mkdir $(BASE_DIR)
	mkdir $(INCL_DIR)

/usr/local/include/aossl/factory.h: lib/include/factory.h
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

/usr/local/include/aossl/xredis_admin.h: lib/include/xredis_admin.h
	cp $< $@

/usr/local/include/aossl/zmqio.h: lib/include/zmqio.h
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

/usr/local/lib/libaossl.a: libaossl.a
	cp $< $@

# Generate Benchmarks
consul_benchmark: lib/consul_benchmark.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ lib/consul_benchmark.o $(OBJS) $(FULL_LIBS) $(STD)

lib/consul_benchmark.o: lib/consul_benchmark.cpp lib/include/factory/consul_interface.h lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/consul_benchmark.cpp $(STD)

couchbase_benchmark: lib/couchbase_benchmark.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ lib/couchbase_benchmark.o $(OBJS) $(FULL_LIBS) $(STD)

lib/couchbase_benchmark.o: lib/couchbase_benchmark.cpp lib/include/factory/couchbase_interface.h lib/include/factory/db_admin.h lib/include/factory/writeable.h lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/couchbase_benchmark.cpp $(STD)

http_benchmark: lib/http_benchmark.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ lib/http_benchmark.o $(OBJS) $(FULL_LIBS) $(STD)

lib/http_benchmark.o: lib/http_benchmark.cpp lib/include/factory/http_interface.h lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/http_benchmark.cpp $(STD)

logging_benchmark: lib/logging_benchmark.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ lib/logging_benchmark.o $(OBJS) $(FULL_LIBS) $(STD)

lib/logging_benchmark.o: lib/logging_benchmark.cpp lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/logging_benchmark.cpp $(STD)

redis_benchmark: lib/redis_benchmark.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ lib/redis_benchmark.o $(OBJS) $(FULL_LIBS) $(STD)

lib/redis_benchmark.o: lib/redis_benchmark.cpp lib/include/factory/redis_interface.h lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/redis_benchmark.cpp $(STD)

uuid_benchmark: lib/uuid_benchmark.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ lib/uuid_benchmark.o $(OBJS) $(FULL_LIBS) $(STD)

lib/uuid_benchmark.o: lib/uuid_benchmark.cpp lib/include/factory/uuid_interface.h lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/uuid_benchmark.cpp $(STD)

zmqio_benchmark: lib/zmqio_benchmark.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ lib/zmqio_benchmark.o $(OBJS) $(FULL_LIBS) $(STD)

lib/zmqio_benchmark.o: lib/zmqio_benchmark.cpp lib/include/factory/zmq_interface.h lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/zmqio_benchmark.cpp $(STD)

# Create the executable file cli_test
cli_test: lib/cli_test.o lib/cli.o
	$(CC) $(CFLAGS) -o $@ lib/cli_test.o lib/cli.o $(STD)

# Create the object file cli_test.o
lib/cli_test.o: lib/cli_test.cpp lib/include/cli.h
	$(CC) $(CFLAGS) -o $@ -c lib/cli_test.cpp $(STD)

# Create the executable file consul_test
consul_test: lib/logging.o lib/consul_test.o lib/consul_admin.o lib/http_admin.o lib/logging_interface.o lib/service.o
	$(CC) $(CFLAGS) -o $@ -o $@ lib/logging.o lib/consul_test.o lib/consul_admin.o lib/http_admin.o lib/logging_interface.o lib/service.o $(LIBS) -lcurl $(STD)

# Create the object file consul_test.o
lib/consul_test.o: lib/consul_test.cpp lib/include/consul_admin.h lib/include/logging.h lib/include/http_admin.h lib/include/factory/logging_interface.h lib/include/service.h
	$(CC) $(CFLAGS) -o $@ -c lib/consul_test.cpp $(STD)

# Create the executable file couchbase_test
couchbase_test: lib/logging.o lib/couchbase_admin.o lib/couchbase_test.o lib/logging_interface.o
	$(CC) $(CFLAGS) -o $@ lib/logging.o lib/couchbase_admin.o lib/couchbase_test.o lib/logging_interface.o $(LIBS) -lcouchbase $(STD)

# Create the object file couchbase_test.o
lib/couchbase_test.o: lib/couchbase_test.cpp lib/include/couchbase_admin.h lib/include/logging.h lib/include/factory/db_admin.h lib/include/factory/writeable.h lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/couchbase_test.cpp $(STD)

# Create the executable file http_test
http_test: lib/logging.o lib/http_admin.o lib/http_test.o lib/logging_interface.o
	$(CC) $(CFLAGS) -o $@ lib/logging.o lib/http_test.o lib/http_admin.o lib/logging_interface.o $(LIBS) -lcurl $(STD)

# Create the object file http_test.o
lib/http_test.o: lib/http_test.cpp lib/include/http_admin.h lib/include/logging.h lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/http_test.cpp $(STD)

# Create the executable file logging_test
logging_test: lib/logging.o lib/logging_test.o lib/logging_interface.o
	$(CC) $(CFLAGS) -o $@ lib/logging_test.o lib/logging.o lib/logging_interface.o $(LIBS) $(STD)

# Create the object file logging_test.o
lib/logging_test.o: lib/logging_test.cpp lib/include/logging.h lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/logging_test.cpp $(STD)

# Create the executable file redis_test
redis_test: lib/logging.o lib/xredis_admin.o lib/redis_test.o lib/logging_interface.o
	$(CC) $(CFLAGS) -o $@ lib/logging.o lib/xredis_admin.o lib/redis_test.o lib/logging_interface.o $(LIBS) -lxredis `pkg-config --cflags --libs hiredis` $(STD)

# Create the object file redis_test.o
lib/redis_test.o: lib/redis_test.cpp lib/include/xredis_admin.h lib/include/logging.h lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/redis_test.cpp $(STD)

# Create the executable file uuid_test
uuid_test: lib/logging.o lib/uuid_admin.o lib/uuid_test.o lib/logging_interface.o
	$(CC) $(CFLAGS) -o $@ lib/logging.o lib/uuid_admin.o lib/uuid_test.o lib/logging_interface.o $(LIBS) -luuid $(STD)

# Create the object file uuid_test.o
lib/uuid_test.o: lib/uuid_test.cpp lib/include/uuid_admin.h lib/include/logging.h lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/uuid_test.cpp $(STD)

# Create the executable file zmqio_test
zmqio_test: lib/logging.o lib/zmqio.o lib/zmqio_test.o lib/logging_interface.o
	$(CC) $(CFLAGS) -o $@ lib/logging.o lib/zmqio.o lib/zmqio_test.o lib/logging_interface.o $(LIBS) -lzmq $(STD)

# Create the object file zmqio_test.o
lib/zmqio_test.o: lib/zmqio_test.cpp lib/include/zmqio.h lib/include/logging.h lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/zmqio_test.cpp $(STD)

factory_test: lib/factory_test.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ lib/factory_test.o $(OBJS) $(FULL_LIBS) $(STD)

lib/factory_test.o: lib/factory_test.cpp lib/factory.cpp lib/include/factory.h
	$(CC) $(CFLAGS) -o $@ -c lib/factory_test.cpp $(STD)

# To create the static library we need the object files
# Using $@ gives us the current target
#
libaossl.a:  $(OBJS)
	$(SLC) $@ $(OBJS)

# Create the object file consul_admin.o
lib/consul_admin.o:  lib/consul_admin.cpp lib/include/factory/logging_interface.h lib/include/http_admin.h lib/include/service.h lib/include/factory/consul_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/consul_admin.cpp $(STD)

lib/service.o: lib/service.cpp lib/include/service.h lib/include/factory/consul_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/service.cpp $(STD)

# Create the object file xredis_admin.o
lib/xredis_admin.o:  lib/xredis_admin.cpp lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/xredis_admin.cpp $(STD)

# Create the object file couchbase_admin.o
lib/couchbase_admin.o:  lib/couchbase_admin.cpp lib/include/logging.h lib/include/factory/db_admin.h lib/include/factory/writeable.h
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
lib/cli.o:  lib/cli.cpp lib/include/cli.h
	$(CC) $(CFLAGS) -o $@ -c lib/cli.cpp $(STD)

lib/factory.o: lib/factory.cpp lib/include/factory.h lib/include/zmqio.h lib/include/couchbase_admin.h lib/include/consul_admin.h lib/include/logging.h lib/include/http_admin.h lib/include/uuid_admin.h lib/include/xredis_admin.h lib/include/cli.h lib/include/factory/commandline_interface.h lib/include/factory/consul_interface.h lib/include/factory/couchbase_interface.h lib/include/factory/db_admin.h lib/include/factory/http_interface.h lib/include/factory/logging_interface.h lib/include/factory/redis_interface.h lib/include/factory/uuid_interface.h lib/include/factory/writeable.h lib/include/factory/zmq_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/factory.cpp $(STD)

lib/logging_interface.o: lib/logging_interface.cpp lib/include/factory/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/logging_interface.cpp $(STD)

clean_local:
	$(RM) libaossl.a lib/*.o *~

clean_tests:
	$(RM) *_test

clean_benchmarks:
	$(RM) *_benchmark

clean_install:
	$(RM) -r /usr/local/include/aossl
	$(RM) /usr/local/lib/libaossl.a
