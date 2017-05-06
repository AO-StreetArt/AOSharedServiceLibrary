# Makefile Variables
CC = g++
CFLAGS  = -g -Wall
STD = -std=c++11
OBJS = src/app_log.o src/configuration_manager.o src/globals.o main.o
TESTS = configuration_test
FULL_LIBS = -laossl -lcurl -lpthread -lzmq -lneo4j-client -lssl -lcrypto -lm -llog4cpp -luuid -lmongoc-1.0 -lbson-1.0 -lhiredis `pkg-config --cflags --libs protobuf`
INCL_DIRS = -I/usr/include/libbson-1.0 -I/usr/local/include/libmongoc-1.0 -I/usr/local/include/libbson-1.0
LINK_DIRS = -L/usr/local/lib
RM = rm -f

.PHONY: mksrc mktest

# Central Targets

all: src main.o main

mksrc:
	@$(MAKE) -C src

main.o: main.cpp src/include/app_utils.h
	$(CC) $(CFLAGS) -o $@ -c main.cpp $(STD) $(INCL_DIRS)

main:
	$(CC) $(CFLAGS) -o app $(OBJS) $(FULL_LIBS) $(STD) $(INCL_DIRS) $(LINK_DIRS)

# Tests

test: mktest configuration_test

mktest:
	@$(MAKE) -C tests

configuration_test:
	$(CC) $(CFLAGS) -o configuration_test src/app_log.o src/configuration_manager.o tests/configuration_test.o $(FULL_LIBS) $(STD) -Isrc/include

# Cleanup

clean:
	$(RM) app *.o src/*.o *~ *.log *.log.* *_test tests/*.o

