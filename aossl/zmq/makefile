# Makefile Variables
CC = g++
CFLAGS  = -g -Wall
STD = -std=c++11
INCL = $(INSTALL_DIR)/zmq/include/factory_zmq.h	$(INSTALL_DIR)/zmq/include/zmqio.h $(INSTALL_DIR)/zmq/include/zmq_interface.h
INCL_DIR = $(INSTALL_DIR)/zmq
LIBS = -lzmq
RM = rm -f

#Makefile Targets
default: zmqio.o

tests: zmqio_test zmq_test_client zmq_test_server

benchmark: zmqio_benchmark

install: $(INCL_DIR) $(INCL)

clean:
	$(RM) *.o *~ *_test *_benchmark *test_server *test_client

# Makefile Instructions

# Install
$(INCL_DIR):
	mkdir $@
	mkdir $@/include

$(INSTALL_DIR)/zmq/include/factory_zmq.h: include/factory_zmq.h
	cp $< $@

$(INSTALL_DIR)/zmq/include/zmqio.h: include/zmqio.h
	cp $< $@

$(INSTALL_DIR)/zmq/include/zmq_interface.h: include/zmq_interface.h
	cp $< $@

# Test
# Create the executable file zmqio_test
zmqio_test: zmqio.o zmqio_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD)

# Create the object file zmqio_test.o
zmqio_test.o: zmqio_test.cpp
	$(CC) $(CFLAGS) -o $@ -c zmqio_test.cpp $(STD)

# Create the executable file zmq_test_client
zmq_test_client: zmqio.o zmq_test_client.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD)

# Create the object file zmq_test_client.o
zmq_test_client.o: zmq_test_client.cpp
	$(CC) $(CFLAGS) -o $@ -c zmq_test_client.cpp $(STD)

# Create the executable file zmq_test_server
zmq_test_server: zmqio.o zmq_test_server.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD)

# Create the object file zmq_test_server.o
zmq_test_server.o: zmq_test_server.cpp
	$(CC) $(CFLAGS) -o $@ -c zmq_test_server.cpp $(STD)

# Benchmark
zmqio_benchmark: zmqio_benchmark.o zmqio.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD)

zmqio_benchmark.o: zmqio_benchmark.cpp
	$(CC) $(CFLAGS) -o $@ -c $^ $(STD)

# Build ZMQ Object file
zmqio.o: zmqio.cpp include/zmqio.h include/zmq_interface.h
	$(CC) $(CFLAGS) -o $@ -c zmqio.cpp $(STD)
