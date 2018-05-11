# Makefile variables
#
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#

#Compilation Variables
CC = g++
SLC = ar rcs
CFLAGS  = -g -Wall
STD = -std=c++11

#Install Variables
export INSTALL_DIR = /usr/local/include/aossl
INSTALL_LIB_DIR = /usr/local/lib

#Library Objects
OBJS = aossl/commandline/cli.o aossl/http/client/http_admin.o\
	aossl/consul/consul_admin.o aossl/uuid/uuid_admin.o \
		aossl/consul/service.o


# typing 'make' will invoke the first target entry in the file
# (in this case the default target entry)
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention
#
default: make_subfolders
	$(SLC) libaossl.a $(OBJS)

# typing 'make docs' will build the html and latex documentation
docs:
	cd docs && $(MAKE) html

# typing 'make tests' will build the tests
tests:
	cd aossl && $(MAKE) tests

# typing 'make benchmarks' will build the benchmarks
benchmarks:
	cd aossl && $(MAKE) benchmarks

# typing 'sudo make install' will install the libraries into system paths
install: $(INSTALL_DIR) $(INSTALL_LIB_DIR)/libaossl.a install_subfolders

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

# To remove the installed library, type 'make uninstall'.
uninstall:
	$(RM) -r $(INSTALL_DIR)
	$(RM) $(INSTALL_LIB_DIR)/libaossl.a

make_subfolders:
	cd aossl && $(MAKE)
