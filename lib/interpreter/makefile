# Makefile Variables
OBJS = cli/cli.o properties_reader/properties_reader.o
INCL = /usr/local/include/aossl/interpreter /usr/local/include/aossl/interpreter/interpreter.h

# Execute the makefile of each interpreter subdirectory
default: $(OBJS)

cli/cli.o:
	cd cli && $(MAKE)

properties_reader/properties_reader.o:
	cd properties_reader && $(MAKE)

# Step into each subdirectory and make clean
clean:
	cd cli && $(MAKE) clean
	cd properties_reader && $(MAKE) clean

# Install the library
install: $(INCL)

/usr/local/include/aossl/interpreter:
	mkdir $@ install_subdirs

/usr/local/include/aossl/interpreter/interpreter.h: interpreter.h
	cp $< $@

install_subdirs:
	cd cli && $(MAKE) install
	cd properties_reader && $(MAKE) install
