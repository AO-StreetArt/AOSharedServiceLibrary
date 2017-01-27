# Makefile Variables
CC = g++
CFLAGS  = -g -Wall
STD = -std=c++11
INCL = /usr/local/include/aossl/neo4j/factory_neo4j.h	/usr/local/include/aossl/neo4j/neo4j_admin.h /usr/local/include/aossl/neo4j/neo4j_interface.h
INCL_DIR = /usr/local/include/aossl/neo4j
LIBS = -lneo4j-client -lssl -lcrypto -lm
RM = rm

#Makefile Targets
default: neo4j_admin.o

test: neo4j_test

install: $(INCL_DIR) $(INCL)

clean:
	$(RM) *.o *~ *_test *_benchmark

# Makefile Instructions
# Create the object file neo4j_admin.o
neo4j_admin.o:  neo4j_admin.cpp include/neo4j_admin.h include/neo4j_interface.h
	$(CC) $(CFLAGS) -o $@ -c neo4j_admin.cpp $(STD)

# Install
/usr/local/include/aossl/neo4j:
	mkdir $@

/usr/local/include/aossl/neo4j/factory_neo4j.h: include/factory_neo4j.h
	cp $< $@

/usr/local/include/aossl/neo4j/neo4j_admin.h: include/neo4j_admin.h
	cp $< $@

/usr/local/include/aossl/neo4j/neo4j_interface.h: include/neo4j_interface.h
	cp $< $@

# Test
# Create the executable file zmqio_test
neo4j_test: neo4j_admin.o neo4j_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD)

# Create the object file zmqio_test.o
neo4j_test.o: neo4j_test.cpp
	$(CC) $(CFLAGS) -o $@ -c neo4j_test.cpp $(STD)
