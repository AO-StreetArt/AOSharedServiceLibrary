# Makefile Variables
CC = g++
CFLAGS  = -g -Wall
STD = -std=c++11
INCL = /usr/local/include/aossl/mongo/factory_mongo.h	/usr/local/include/aossl/mongo/mongo_admin.h /usr/local/include/aossl/mongo/mongo_interface.h
INCL_DIR = /usr/local/include/aossl/mongo
LIBS = -lmongoc-1.0 -lbson-1.0
INCL_DIRS = -I/usr/include/libbson-1.0 -I/usr/local/include/libmongoc-1.0 -I/usr/local/include/libbson-1.0
LINK_DIRS = -L/usr/local/lib
RM = rm

#User Targets
default: mongo_admin.o

test: mongo_test

install: $(INCL_DIR) $(INCL)

clean:
	$(RM) *.o *~ *_test *_benchmark

# Makefile Instructions
# Create the object file mongo_admin.o
mongo_admin.o:  mongo_admin.cpp include/mongo_admin.h include/mongo_interface.h
	$(CC) $(CFLAGS) -o $@ -c mongo_admin.cpp $(STD) $(INCL_DIRS)

# Install Targets
/usr/local/include/aossl/mongo:
	mkdir $@

/usr/local/include/aossl/mongo/factory_mongo.h: include/factory_mongo.h
	cp $< $@

/usr/local/include/aossl/mongo/mongo_admin.h: include/mongo_admin.h
	cp $< $@

/usr/local/include/aossl/mongo/mongo_interface.h: include/mongo_interface.h
	cp $< $@

# Test targets
# Create the executable file zmqio_test
mongo_test: mongo_admin.o mongo_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD) $(INCL_DIRS) $(LINK_DIRS)

# Create the object file zmqio_test.o
mongo_test.o: mongo_test.cpp
	$(CC) $(CFLAGS) -o $@ -c mongo_test.cpp $(STD) $(INCL_DIRS)
