# Makefile Variables
INCL = $(INSTALL_DIR)/properties/include/factory_props.h $(INSTALL_DIR)/properties/include/properties_reader.h
INCL_DIR = $(INSTALL_DIR)/properties
INCL_DIRS = -I$(CURDIR)/../../

#Makefile Targets
tests: properties_reader_test

install: $(INCL_DIR) $(INCL)

clean:
	$(RM) *.o *~ *_test *_benchmark

# Makefile Instructions

# Install
$(INSTALL_DIR)/properties:
	mkdir $@
	mkdir $@/include

$(INSTALL_DIR)/properties/include/factory_props.h: include/factory_props.h
	cp $< $@

$(INSTALL_DIR)/properties/include/properties_reader.h: include/properties_reader.h
	cp $< $@

# Test
# Create the executable file zmqio_test
properties_reader_test: properties_reader_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD) $(INCL_DIRS)

properties_reader_test.o: properties_reader_test.cpp include/properties_reader.h
	$(CC) $(CFLAGS) -o $@ -c properties_reader_test.cpp $(STD) $(INCL_DIRS)
