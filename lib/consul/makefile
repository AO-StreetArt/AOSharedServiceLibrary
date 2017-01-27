# Makefile Variables
CC = g++
CFLAGS  = -g -Wall
STD = -std=c++11
INSTALL_FILES = /usr/local/include/aossl/consul/factory_consul.h	/usr/local/include/aossl/consul/consul_admin.h /usr/local/include/aossl/consul/consul_interface.h /usr/local/include/aossl/consul/service.h
INSTALL_DIR = /usr/local/include/aossl/consul
LIBS = -lcurl
RM = rm
OBJS = consul_admin.o service.o
INCL_OBJS = $(CURDIR)/../../http/client/http_admin.o
INCL_DIRS = -I$(CURDIR)/../../http/client/include/

#Makefile Targets
default: $(OBJS)

test: consul_test

benchmark: consul_benchmark

install: $(INSTALL_DIR) $(INSTALL_FILES)

clean:
	$(RM) *.o *~ *_test *_benchmark

# Makefile Instructions
# Create the object file consul_admin.o
consul_admin.o:  consul_admin.cpp include/consul_admin.h include/consul_interface.h
	$(CC) $(CFLAGS) -o $@ -c consul_admin.cpp $(STD) $(INCL_DIRS)

service.o: service.cpp include/service.h include/consul_interface.h
	$(CC) $(CFLAGS) -o $@ -c lib/service.cpp $(STD) $(INCL_DIRS)

# Install
$(INSTALL_DIR):
	mkdir $@

/usr/local/include/aossl/consul/factory_consul.h: include/factory_consul.h
	cp $< $@

/usr/local/include/aossl/consul/consul_admin.h: include/consul_admin.h
	cp $< $@

/usr/local/include/aossl/consul/consul_interface.h: include/consul_interface.h
	cp $< $@

/usr/local/include/aossl/consul/service.h: include/service.h
	cp $< $@

# Test
# Create the executable file zmqio_test
consul_test: consul_test.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD) $(INCL_OBJS) $(INCL_DIRS)

# Create the object file zmqio_test.o
consul_test.o: consul_test.cpp
	$(CC) $(CFLAGS) -o $@ -c consul_test.cpp $(STD) $(INCL_DIRS)

# Benchmark
consul_benchmark: consul_benchmark.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD) $(INCL_OBJS) $(INCL_DIRS)

lib/consul_benchmark.o: consul_benchmark.cpp
	$(CC) $(CFLAGS) -o $@ -c $^ $(STD) $(INCL_DIRS)
