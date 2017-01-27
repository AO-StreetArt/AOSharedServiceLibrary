# Makefile Variables
CC = g++
CFLAGS  = -g -Wall
STD = -std=c++11
INCL = /usr/local/include/aossl/http/client/factory_http_client.h	/usr/local/include/aossl/http/client/http_admin.h /usr/local/include/aossl/http/client/http_interface.h
INCL_DIR = /usr/local/include/aossl/http/client
LIBS = -lcurl
RM = rm

#Makefile Targets
default: http_admin.o

test: http_test

benchmark: http_benchmark

install: $(INCL_DIR) $(INCL)

clean:
	$(RM) *.o *~ *_test *_benchmark

# Makefile Instructions
# Create the object file http_admin.o
http_admin.o:  http_admin.cpp include/http_admin.h include/http_interface.h
	$(CC) $(CFLAGS) -o $@ -c http_admin.cpp $(STD)

# Install
/usr/local/include/aossl/http:
	mkdir $@

/usr/local/include/aossl/http/factory_http_client.h: include/factory_http_client.h
	cp $< $@

/usr/local/include/aossl/http/http_admin.h: include/http_admin.h
	cp $< $@

/usr/local/include/aossl/http/http_interface.h: include/http_interface.h
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

lib/http_benchmark.o: http_benchmark.cpp
	$(CC) $(CFLAGS) -o $@ -c $^ $(STD)
