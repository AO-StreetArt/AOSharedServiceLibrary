# Makefile Variables
OBJS = client/http_admin.o server/http_server.o

# Execute the makefile of each interpreter subdirectory
default: $(OBJS)

client/http_admin.o:
	cd client && $(MAKE)

server/http_server.o:
	cd server && $(MAKE)

# tests
tests:
	cd client && $(MAKE) tests
	cd server && $(MAKE) tests

# benchmarks
benchmark:
	cd client && $(MAKE) benchmark

# Step into each subdirectory and make clean
clean:
	cd client && $(MAKE) clean
	cd server && $(MAKE) clean

# Install the library
install: $(INSTALL_DIR)/http install_subdirs

$(INSTALL_DIR)/http:
	mkdir $@

install_subdirs:
	cd client && $(MAKE) install
	cd server && $(MAKE) install
