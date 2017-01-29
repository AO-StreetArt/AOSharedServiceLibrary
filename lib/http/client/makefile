# Makefile Variables
CC = g++
CFLAGS  = -g -Wall
STD = -std=c++11
INCL = $(INSTALL_DIR)/http/client/factory_http_client.h	$(INSTALL_DIR)/http/client/http_admin.h $(INSTALL_DIR)/http/client/http_interface.h
INCL_DIR = $(INSTALL_DIR)/http/client
LIBS = -lcurl
RM = rm -f

#Makefile Targets
default: http_admin.o

tests: http_test

benchmark: http_benchmark

install: $(INCL_DIR) $(INCL)

$(INCL_DIR):
	mkdir $@

clean:
	$(RM) *.o *~ *_test *_benchmark

# Makefile Instructions
# Create the object file http_admin.o
http_admin.o:  http_admin.cpp include/http_admin.h include/http_interface.h
	$(CC) $(CFLAGS) -o $@ -c http_admin.cpp $(STD)

# Install
$(INSTALL_DIR)/http:
	mkdir $@

$(INSTALL_DIR)/http/client/factory_http_client.h: include/factory_http_client.h
	cp $< $@

$(INSTALL_DIR)/http/client/http_admin.h: include/http_admin.h
	cp $< $@

$(INSTALL_DIR)/http/client/http_interface.h: include/http_interface.h
	cp $< $@

# Test
# Create the executable file zmqio_test
http_test: http_admin.o http_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD)

# Create the object file zmqio_test.o
http_test.o: http_test.cpp
	$(CC) $(CFLAGS) -o $@ -c http_test.cpp $(STD)

# Benchmark
http_benchmark: http_benchmark.o http_admin.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD)

http_benchmark.o: http_benchmark.cpp
	$(CC) $(CFLAGS) -o $@ -c $^ $(STD)
