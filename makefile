# Makefile variables
#
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#

.PHONY: examples

#Compilation Variables
CC = g++
SLC = ar rcs
CFLAGS  = -g -Wall
STD = -std=c++11

#Install Variables
export INSTALL_DIR = /usr/local/include/aossl
INSTALL_LIB_DIR = /usr/local/lib

#Library Objects
OBJS = aossl/commandline/cli.o aossl/logging/logging.o aossl/http/client/http_admin.o\
 aossl/zmq/zmqio.o aossl/redis/redis_admin.o aossl/consul/consul_admin.o aossl/logging/logging_interface.o\
  aossl/uuid/uuid_admin.o aossl/consul/service.o aossl/http/server/http_server.o\
	 aossl/properties/properties_reader.o aossl/mongo/mongo_admin.o aossl/neo4j/neo4j_admin.o


# typing 'make' will invoke the first target entry in the file
# (in this case the default target entry)
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention
#
default: libaossl.a

# typing 'make tests' will build the tests
tests:
	cd aossl && $(MAKE) tests

# 'make rhel-test' will build the tests on RHEL/CentOS
rhel-test:
	cd aossl && $(MAKE) rhel-test

# typing 'make benchmarks' will build the benchmarks
benchmarks:
	cd aossl && $(MAKE) benchmarks

# typing 'make rhel-benchmarks' will build benchmarks on RHEL/CentOS
rhel-benchmarks:
	cd aossl && $(MAKE) rhel-benchmarks

# typing 'sudo make install' will install the libraries into system paths
install: $(INSTALL_DIR) $(INSTALL_LIB_DIR)/libaossl.a install_subfolders

# 'make examples' will build all of the examples
examples:
	@$(MAKE) -C examples

$(INSTALL_DIR):
	mkdir $@

$(INSTALL_LIB_DIR)/libaossl.a: libaossl.a
	cp $< $@

install_subfolders:
	cd aossl && $(MAKE) install

# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean:
	$(RM) libaossl.a *~
	cd aossl && $(MAKE) clean
	cd examples && $(MAKE) clean

# To remove the installed library, type 'make uninstall'.
uninstall:
	$(RM) -r $(INSTALL_DIR)
	$(RM) $(INSTALL_LIB_DIR)/libaossl.a

# To create the static library we need the object files
# Using $@ gives us the current target
#
libaossl.a:  make_subfolders
	$(SLC) $@ $(OBJS)

make_subfolders:
	cd aossl && $(MAKE)
