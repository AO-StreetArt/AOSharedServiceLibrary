# Makefile Variables
CC = g++
CFLAGS  = -g -Wall
STD = -std=c++11
INCL = /usr/local/include/aossl/properties_reader/factory_props.h	/usr/local/include/aossl/properties_reader/properties_reader_interface.h /usr/local/include/aossl/properties_reader/properties_reader.h
INCL_DIR = /usr/local/include/aossl/properties_reader
RM = rm

#Makefile Targets
default: properties_reader.o

test: properties_reader_test

install: $(INCL_DIR) $(INCL)

clean:
	$(RM) *.o *~ *_test *_benchmark

# Makefile Instructions

# Install
/usr/local/include/aossl/properties_reader:
	mkdir $@

/usr/local/include/aossl/properties_reader/factory_properties_reader.h: include/factory_properties_reader.h
	cp $< $@

/usr/local/include/aossl/properties_reader/properties_reader_interface.h: include/properties_reader.h
	cp $< $@

/usr/local/include/aossl/properties_reader/properties_reader_interface.h: include/properties_reader_interface.h
	cp $< $@

# Test
# Create the executable file zmqio_test
properties_reader_test: properties_readero properties_reader_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD)

# Create the object file zmqio_test.o
properties_reader: properties_reader_test.o: properties_reader_test.cpp
	$(CC) $(CFLAGS) -o $@ -c properties_reader_test.cpp $(STD)

# Build ZMQ Object file
properties_reader.o: properties_reader.cpp include/properties_reader.h include/properties_reader_interface.h
	$(CC) $(CFLAGS) -o $@ -c properties_reader.cpp $(STD)
