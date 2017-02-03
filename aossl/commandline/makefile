# Makefile Variables
CC = g++
CFLAGS  = -g -Wall
STD = -std=c++11
INCL = $(INSTALL_DIR)/commandline/include/factory_cli.h	$(INSTALL_DIR)/commandline/include/cli.h $(INSTALL_DIR)/commandline/include/commandline_interface.h
INCL_DIR = $(INSTALL_DIR)/commandline
RM = rm -f
INCL_DIRS = -I$(CURDIR)/../

#Makefile Targets
default: cli.o

tests: cli_test

install: $(INCL_DIR) $(INCL)

clean:
	$(RM) *.o *~ *_test *_benchmark

# Makefile Instructions

# Install
$(INSTALL_DIR)/commandline:
	mkdir $@
	mkdir $@/include

$(INSTALL_DIR)/commandline/include/factory_cli.h: include/factory_cli.h
	cp $< $@

$(INSTALL_DIR)/commandline/include/cli.h: include/cli.h
	cp $< $@

$(INSTALL_DIR)/commandline/include/commandline_interface.h: include/commandline_interface.h
	cp $< $@

# Test
# Create the executable file cli_test
cli_test: cli.o cli_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD) $(INCL_DIRS)

# Create the object file cli_test.o
cli_test.o: cli_test.cpp
	$(CC) $(CFLAGS) -o $@ -c cli_test.cpp $(STD) $(INCL_DIRS)

# Build ZMQ Object file
cli.o: cli.cpp include/cli.h include/commandline_interface.h
	$(CC) $(CFLAGS) -o $@ -c cli.cpp $(STD) $(INCL_DIRS)
