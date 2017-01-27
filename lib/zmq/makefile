# Makefile Variables
CC = g++
CFLAGS  = -g -Wall
STD = -std=c++11
INCL = /usr/local/include/aossl/zmq/factory_zmq.h	/usr/local/include/aossl/zmq/zmqio.h /usr/local/include/aossl/zmq/zmq_interface.h
INCL_DIR = /usr/local/include/aossl/zmq
LIBS = -lzmq
RM = rm

#Makefile Targets
default: zmqio.o

test: zmqio_test

benchmark: zmqio_benchmark

install: $(INCL_DIR) $(INCL)

clean:
	$(RM) *.o *~ *_test *_benchmark

# Makefile Instructions

# Install
/usr/local/include/aossl/zmq:
	mkdir $@

/usr/local/include/aossl/zmq/factory_zmq.h: include/factory_zmq.h
	cp $< $@

/usr/local/include/aossl/zmq/zmqio.h: include/zmqio.h
	cp $< $@

/usr/local/include/aossl/zmq/zmq_interface.h: include/zmq_interface.h
	cp $< $@

# Test
# Create the executable file zmqio_test
zmqio_test: zmqio.o zmqio_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD)

# Create the object file zmqio_test.o
zmqio_test.o: zmqio_test.cpp
	$(CC) $(CFLAGS) -o $@ -c zmqio_test.cpp $(STD)

# Benchmark
zmqio_benchmark: zmqio_benchmark.o zmqio.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD)

zmqio_benchmark.o: zmqio_benchmark.cpp
	$(CC) $(CFLAGS) -o $@ -c $^ $(STD)

# Build ZMQ Object file
zmqio.o: zmqio.cpp include/zmqio.h include/zmq_interface.h
	$(CC) $(CFLAGS) -o $@ -c zmqio.cpp $(STD)
