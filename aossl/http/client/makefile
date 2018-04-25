# Makefile Variables
CC = g++
CFLAGS  = -g -Wall
STD = -std=c++11
INCL = $(INSTALL_DIR)/http/client/include/factory_http_client.h	$(INSTALL_DIR)/http/client/include/http_admin.h $(INSTALL_DIR)/http/client/include/http_interface.h
INCL_DIR = $(INSTALL_DIR)/http/client
LIBS = -lcurl
RM = rm -f
INCL_DIRS = -I$(CURDIR)/../../../

#Makefile Targets
default: http_admin.o

tests: http_test

benchmark: http_benchmark

install: $(INCL_DIR)/include $(INCL)

$(INCL_DIR)/include: $(INCL_DIR)
	mkdir $@

$(INCL_DIR):
	mkdir $@

clean:
	$(RM) *.o *~ *_test *_benchmark

# Makefile Instructions
# Create the object file http_admin.o
http_admin.o:  http_admin.cpp include/http_admin.h include/http_interface.h
	$(CC) $(CFLAGS) -o $@ -c http_admin.cpp $(STD) $(INCL_DIRS)

# Install
$(INSTALL_DIR)/http/client:
	mkdir $@

$(INSTALL_DIR)/http/client/include/factory_http_client.h: include/factory_http_client.h
	cp $< $@

$(INSTALL_DIR)/http/client/include/http_admin.h: include/http_admin.h
	cp $< $@

$(INSTALL_DIR)/http/client/include/http_interface.h: include/http_interface.h
	cp $< $@

# Test
# Create the executable file zmqio_test
http_test: http_admin.o http_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD) $(INCL_DIRS)

# Create the object file zmqio_test.o
http_test.o: http_test.cpp
	$(CC) $(CFLAGS) -o $@ -c http_test.cpp $(STD) $(INCL_DIRS)

# Benchmark
http_benchmark: http_benchmark.o http_admin.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD) $(INCL_DIRS)

http_benchmark.o: http_benchmark.cpp
	$(CC) $(CFLAGS) -o $@ -c $^ $(STD) $(INCL_DIRS)
