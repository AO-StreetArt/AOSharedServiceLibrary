# Makefile Variables
OBJS = client/http_admin.o

# Execute the makefile of each interpreter subdirectory
default: $(OBJS)

client/http_admin.o:
	cd client && $(MAKE)

# tests
tests:
	cd client && $(MAKE) tests

# benchmarks
benchmark:
	cd client && $(MAKE) benchmark

# Step into each subdirectory and make clean
clean:
	cd client && $(MAKE) clean

# Install the library
install: $(INSTALL_DIR)/http install_subdirs

$(INSTALL_DIR)/http:
	mkdir $@

install_subdirs:
	cd client && $(MAKE) install
