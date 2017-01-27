# Makefile Variables
CC = g++
CFLAGS  = -g -Wall
STD = -std=c++11
INCL = /usr/local/include/aossl/logging/factory_logging.h	/usr/local/include/aossl/logging/logging.h /usr/local/include/aossl/logging/logging_interface.h
INCL_DIR = /usr/local/include/aossl/logging
LIBS = -lpthread -llog4cpp
RM = rm

#Makefile Targets
default: logging.o

test: logging_test

benchmark: logging_benchmark

install: $(INCL_DIR) $(INCL)

clean:
	$(RM) *.o *~ *_test *_benchmark

# Makefile Instructions

# Install
/usr/local/include/aossl/logging:
	mkdir $@

/usr/local/include/aossl/logging/factory_logging.h: include/factory_logging.h
	cp $< $@

/usr/local/include/aossl/logging/logging.h: include/logging.h
	cp $< $@

/usr/local/include/aossl/logging/logging_interface.h: include/logging_interface.h
	cp $< $@

# Test
# Create the executable file zmqio_test
logging_test: logging.o logging_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD)

# Create the object file zmqio_test.o
logging_test.o: logging_test.cpp
	$(CC) $(CFLAGS) -o $@ -c logging_test.cpp $(STD)

# Benchmark
logging_benchmark: logging_benchmark.o logging.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD)

logging_benchmark.o: logging_benchmark.cpp
	$(CC) $(CFLAGS) -o $@ -c $^ $(STD)

# Build ZMQ Object file
logging.o: logging.cpp include/logging.h include/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c logging.cpp $(STD)
