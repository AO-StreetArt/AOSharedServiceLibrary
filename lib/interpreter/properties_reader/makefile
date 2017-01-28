# Makefile Variables
CC = g++
CFLAGS  = -g -Wall
STD = -std=c++11
INCL = $(INSTALL_DIR)/properties_reader/factory_props.h	$(INSTALL_DIR)/properties_reader/properties_reader_interface.h $(INSTALL_DIR)/properties_reader/properties_reader.h
INCL_DIR = $(INSTALL_DIR)/properties_reader
RM = rm -f
INCL_DIRS = -I$(CURDIR)/../

#Makefile Targets
default: properties_reader.o

tests: properties_reader_test

install: $(INCL_DIR) $(INCL)

clean:
	$(RM) *.o *~ *_test *_benchmark

# Makefile Instructions

# Install
$(INSTALL_DIR)/properties_reader:
	mkdir $@

$(INSTALL_DIR)/properties_reader/factory_props.h: include/factory_props.h
	cp $< $@

$(INSTALL_DIR)/properties_reader/properties_reader_interface.h: include/properties_reader.h
	cp $< $@

$(INSTALL_DIR)/properties_reader/properties_reader.h: include/properties_reader.h
	cp $< $@

# Test
# Create the executable file zmqio_test
properties_reader_test: properties_reader.o properties_reader_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD) $(INCL_DIRS)

properties_reader_test.o: properties_reader_test.cpp include/properties_reader.h
	$(CC) $(CFLAGS) -o $@ -c properties_reader_test.cpp $(STD) $(INCL_DIRS)

# Create the object file zmqio_test.o
properties_reader: properties_reader_test.o properties_reader_test.cpp
	$(CC) $(CFLAGS) -o $@ -c properties_reader_test.cpp $(STD) $(INCL_DIRS)

# Build ZMQ Object file
properties_reader.o: properties_reader.cpp include/properties_reader.h include/properties_reader_interface.h
	$(CC) $(CFLAGS) -o $@ -c properties_reader.cpp $(STD) $(INCL_DIRS)
