# Makefile Variables
CC = g++
CFLAGS  = -g -Wall
LIBS = -laossl -lpthread -llog4cpp -levent -lcurl -luuid -lzmq
STD = -std=c++11
OBJS = configuration_manager.o app_log.o

all: $(OBJS)

configuration_manager.o: include/configuration_manager.h configuration_manager.cpp
	$(CC) $(CFLAGS) -o $@ -c configuration_manager.cpp $(STD)

app_log.o: include/app_log.h app_log.cpp
	$(CC) $(CFLAGS) -o $@ -c app_log.cpp $(STD)
