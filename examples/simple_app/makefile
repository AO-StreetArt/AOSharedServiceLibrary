# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
#
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#

CC = g++
CFLAGS  = -g -Wall
STD = -std=c++11
LIBS = -laossl -lpthread -llog4cpp -lzmq -luuid -lxredis -lcurl -lcouchbase `pkg-config --cflags --libs hiredis`

default: simple_app

clean: clean_local

simple_app: simple_app.o
	$(CC) $(CFLAGS) -o $@ simple_app.o $(LIBS) $(STD)

simple_app.o: simple_app.cpp
	$(CC) $(CFLAGS) -o $@ -c simple_app.cpp $(STD)

clean_local:
	$(RM) *_app *.o *~
