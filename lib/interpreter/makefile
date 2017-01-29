# Makefile Variables
OBJS = cli/cli.o properties_reader/properties_reader.o
INCL = $(INSTALL_DIR)/interpreter $(INSTALL_DIR)/interpreter/interpreter.h

# Execute the makefile of each interpreter subdirectory
default: $(OBJS)

cli/cli.o:
	cd cli && $(MAKE)

properties_reader/properties_reader.o:
	cd properties_reader && $(MAKE)

# Tests
tests:
	cd cli && $(MAKE) tests
	cd properties_reader && $(MAKE) tests

# Step into each subdirectory and make clean
clean:
	cd cli && $(MAKE) clean
	cd properties_reader && $(MAKE) clean

# Install the library
install: $(INCL)
	cd cli && $(MAKE) install
	cd properties_reader && $(MAKE) install

$(INSTALL_DIR)/interpreter:
	mkdir $@

$(INSTALL_DIR)/interpreter/interpreter.h: interpreter.h
	cp $< $@
