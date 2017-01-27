# Makefile Variables
CC = g++
CFLAGS  = -g -Wall
STD = -std=c++11
INCL = /usr/local/include/aossl/http/server/factory_http_server.h	/usr/local/include/aossl/http/server/http_server.h /usr/local/include/aossl/http/server/http_server_interface.h /usr/local/include/aossl/http/server/callbacks.h
INCL_DIR = /usr/local/include/aossl/http/server
LIBS = -levent
RM = rm

#Makefile Targets
default: http_server.o

test: http_server_test

install: $(INCL_DIR) $(INCL)

clean:
	$(RM) *.o *~ *_test *_benchmark

# Makefile Instructions
# Create the object file http_server.o
http_server.o:  http_server.cpp include/http_server.h include/http_server_interface.h
	$(CC) $(CFLAGS) -o $@ -c http_server.cpp $(STD)

# Install
/usr/local/include/aossl/http/server:
	mkdir $@

/usr/local/include/aossl/uuid/factory_http_server.h: include/factory_http_server.h
	cp $< $@

/usr/local/include/aossl/uuid/http_server.h: include/http_server.h
	cp $< $@

/usr/local/include/aossl/uuid/http_server_interface.h: include/http_server_interface.h
	cp $< $@

/usr/local/include/aossl/http/server/callbacks.h: include/callbacks.h
	cp $< $@

# Test
# Create the executable file zmqio_test
uuid_test: http_server.o uuid_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD)

# Create the object file zmqio_test.o
uuid_test.o: uuid_test.cpp
	$(CC) $(CFLAGS) -o $@ -c uuid_test.cpp $(STD)
