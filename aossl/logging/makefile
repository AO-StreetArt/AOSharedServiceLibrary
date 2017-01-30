# Makefile Variables
CC = g++
CFLAGS  = -g -Wall
STD = -std=c++11
INCL = $(INSTALL_DIR)/logging/include/factory_logging.h	$(INSTALL_DIR)/logging/include/logging.h $(INSTALL_DIR)/logging/include/logging_interface.h
INCL_DIR = $(INSTALL_DIR)/logging
LIBS = -lpthread -llog4cpp
RM = rm -f

#Makefile Targets
default: logging.o logging_interface.o

tests: logging_test

benchmark: logging_benchmark

install: $(INCL_DIR) $(INCL)

clean:
	$(RM) *.o *~ *_test *_benchmark

# Makefile Instructions

# Install
$(INSTALL_DIR)/logging:
	mkdir $@
	mkdir $@/include

$(INSTALL_DIR)/logging/include/factory_logging.h: include/factory_logging.h
	cp $< $@

$(INSTALL_DIR)/logging/include/logging.h: include/logging.h
	cp $< $@

$(INSTALL_DIR)/logging/include/logging_interface.h: include/logging_interface.h
	cp $< $@

# Test
# Create the executable file logging_test
logging_test: logging.o logging_test.o logging_interface.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD)

# Create the object file logging_test.o
logging_test.o: logging_test.cpp
	$(CC) $(CFLAGS) -o $@ -c logging_test.cpp $(STD)

# Benchmark
logging_benchmark: logging_benchmark.o logging.o logging_interface.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD)

logging_benchmark.o: logging_benchmark.cpp
	$(CC) $(CFLAGS) -o $@ -c $^ $(STD)

# Build Logging Object file
logging.o: logging.cpp include/logging.h include/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c logging.cpp $(STD)

# Build Logging Object file
logging_interface.o: logging_interface.cpp include/logging.h include/logging_interface.h
	$(CC) $(CFLAGS) -o $@ -c logging_interface.cpp $(STD)
