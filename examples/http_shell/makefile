# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
#
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#

.PHONY: mksrc

CC = g++
CFLAGS  = -g -Wall
LIBS = -laossl -lpthread -llog4cpp -levent -lcurl -luuid -lzmq
STD = -std=c++11

default: mksrc app

mksrc:
	@$(MAKE) -C src

clean:
	$(RM) app *.o *~ src/*.o *.log

app: main.o
	$(CC) $(CFLAGS) -o $@ main.o src/app_log.o src/configuration_manager.o $(LIBS) $(STD)

main.o: main.cpp
	$(CC) $(CFLAGS) -o $@ -c main.cpp $(STD)
