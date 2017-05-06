# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
#
CC = g++
CFLAGS  = -g -Wall
STD = -std=c++11
TESTS = configuration_test.o

default: $(TESTS)

configuration_test.o: configuration_test.cpp
	$(CC) $(CFLAGS) -o $@ -c configuration_test.cpp $(STD) -I../src/include
