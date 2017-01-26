# Makefile Variables
CC = g++
CFLAGS  = -g -Wall
STD = -std=c++11
INCL = /usr/local/include/aossl/cli/factory_cli.h	/usr/local/include/aossl/cli/cli.h /usr/local/include/aossl/cli/cli_interface.h
INCL_DIR = /usr/local/include/aossl/cli
RM = rm

#Makefile Targets
default: cli.o

test: cli_test

install: $(INCL_DIR) $(INCL)

clean:
	$(RM) *.o *~ *_test *_benchmark

# Makefile Instructions

# Install
/usr/local/include/aossl/cli:
	mkdir $@

/usr/local/include/aossl/cli/factory_cli.h: include/factory_zmq.h
	cp $< $@

/usr/local/include/aossl/cli/commandline_interface.h: include/zmqio.h
	cp $< $@

/usr/local/include/aossl/cli/cli_interface.h: include/zmq_interface.h
	cp $< $@

# Test
# Create the executable file zmqio_test
cli_test: cli.o cli_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD)

# Create the object file zmqio_test.o
cli_test.o: cli_test.cpp
	$(CC) $(CFLAGS) -o $@ -c zmqio_test.cpp $(STD)

# Build ZMQ Object file
cli.o: cli.cpp include/cli.h include/cli_interface.h
	$(CC) $(CFLAGS) -o $@ -c zmqio.cpp $(STD)
