# Makefile Variables
CC = g++
CFLAGS  = -g -Wall
STD = -std=c++11
INCL = $(INSTALL_DIR)/http/server/factory_http_server.h	$(INSTALL_DIR)/http/server/http_server.h $(INSTALL_DIR)/http/server/http_server_interface.h $(INSTALL_DIR)/http/server/callbacks.h
INCL_DIR = $(INSTALL_DIR)/http/server
LIBS = -levent
RM = rm -f

#Makefile Targets
default: http_server.o

tests: http_server_test

install: $(INCL_DIR) $(INCL)

clean:
	$(RM) *.o *~ *_test *_benchmark

# Makefile Instructions
# Create the object file http_server.o
http_server.o:  http_server.cpp include/http_server.h include/http_server_interface.h
	$(CC) $(CFLAGS) -o $@ -c http_server.cpp $(STD)

# Install
$(INSTALL_DIR)/http/server:
	mkdir $@

$(INSTALL_DIR)/http/server/factory_http_server.h: include/factory_http_server.h
	cp $< $@

$(INSTALL_DIR)/http/server/http_server.h: include/http_server.h
	cp $< $@

$(INSTALL_DIR)/http/server/http_server_interface.h: include/http_server_interface.h
	cp $< $@

$(INSTALL_DIR)/http/server/callbacks.h: include/callbacks.h
	cp $< $@

# Test
# Create the executable file zmqio_test
http_server_test: http_server.o http_server_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD)

# Create the object file zmqio_test.o
http_server_test.o: http_server_test.cpp
	$(CC) $(CFLAGS) -o $@ -c http_server_test.cpp $(STD)
