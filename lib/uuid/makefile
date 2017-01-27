# Makefile Variables
CC = g++
CFLAGS  = -g -Wall
STD = -std=c++11
INCL = /usr/local/include/aossl/uuid/factory_uuid.h	/usr/local/include/aossl/uuid/uuid_admin.h /usr/local/include/aossl/uuid/uuid_interface.h
INCL_DIR = /usr/local/include/aossl/uuid
LIBS = -luuid
RM = rm

#Makefile Targets
default: uuid_admin.o

test: uuid_test

benchmark: uuid_benchmark

install: $(INCL_DIR) $(INCL)

clean:
	$(RM) *.o *~ *_test *_benchmark

# Makefile Instructions
# Create the object file uuid_admin.o
uuid_admin.o:  uuid_admin.cpp include/uuid_admin.h include/uuid_interface.h
	$(CC) $(CFLAGS) -o $@ -c uuid_admin.cpp $(STD)

# Install
/usr/local/include/aossl/uuid:
	mkdir $@

/usr/local/include/aossl/uuid/factory_uuid.h: include/factory_uuid.h
	cp $< $@

/usr/local/include/aossl/uuid/uuid_admin.h: include/uuid_admin.h
	cp $< $@

/usr/local/include/aossl/uuid/uuid_interface.h: include/uuid_interface.h
	cp $< $@

# Test
# Create the executable file zmqio_test
uuid_test: uuid_admin.o uuid_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD)

# Create the object file zmqio_test.o
uuid_test.o: uuid_test.cpp
	$(CC) $(CFLAGS) -o $@ -c uuid_test.cpp $(STD)

# Benchmark
uuid_benchmark: uuid_benchmark.o uuid_admin.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD)

lib/uuid_benchmark.o: uuid_benchmark.cpp
	$(CC) $(CFLAGS) -o $@ -c $^ $(STD)
