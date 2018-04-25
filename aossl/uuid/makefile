# Makefile Variables
CC = g++
CFLAGS  = -g -Wall
STD = -std=c++11
INCL_DIRS = -I$(CURDIR)/../../
INCL = $(INSTALL_DIR)/uuid/include/factory_uuid.h	$(INSTALL_DIR)/uuid/include/uuid_admin.h $(INSTALL_DIR)/uuid/include/uuid_interface.h
INCL_DIR = $(INSTALL_DIR)/uuid
LIBS = -luuid
RM = rm -f

#Makefile Targets
default: uuid_admin.o

tests: uuid_test

benchmark: uuid_benchmark

install: $(INCL_DIR) $(INCL)

clean:
	$(RM) *.o *~ *_test *_benchmark

# Makefile Instructions
# Create the object file uuid_admin.o
uuid_admin.o:  uuid_admin.cpp include/uuid_admin.h include/uuid_interface.h
	$(CC) $(CFLAGS) -o $@ -c uuid_admin.cpp $(STD) $(INCL_DIRS)

# Install
$(INSTALL_DIR)/uuid:
	mkdir $@
	mkdir $@/include

$(INSTALL_DIR)/uuid/include/factory_uuid.h: include/factory_uuid.h
	cp $< $@

$(INSTALL_DIR)/uuid/include/uuid_admin.h: include/uuid_admin.h
	cp $< $@

$(INSTALL_DIR)/uuid/include/uuid_interface.h: include/uuid_interface.h
	cp $< $@

# Test
# Create the executable file zmqio_test
uuid_test: uuid_admin.o uuid_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD) $(INCL_DIRS)

# Create the object file zmqio_test.o
uuid_test.o: uuid_test.cpp
	$(CC) $(CFLAGS) -o $@ -c uuid_test.cpp $(STD) $(INCL_DIRS)

# Benchmark
uuid_benchmark: uuid_benchmark.o uuid_admin.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD) $(INCL_DIRS)

uuid_benchmark.o: uuid_benchmark.cpp
	$(CC) $(CFLAGS) -o $@ -c $^ $(STD) $(INCL_DIRS)
