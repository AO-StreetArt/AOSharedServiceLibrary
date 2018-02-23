# Makefile Variables
CC = g++
CFLAGS  = -g -Wall
STD = -std=c++11
RM = rm -f
INCL = $(INSTALL_DIR)/core/include/buffers.h	$(INSTALL_DIR)/core/include/slot_pool.h $(INSTALL_DIR)/core/include/connection.h $(INSTALL_DIR)/core/include/connection_pool_interface.h
INCL_DIR = $(INSTALL_DIR)/core

# Central Targets
tests: buffer_test slot_pool_test

install: $(INCL_DIR) $(INCL)

# Tests
buffer_test.o: buffer_test.cpp include/buffers.h
	$(CC) $(CFLAGS) -o $@ -c buffer_test.cpp $(STD)

buffer_test: buffer_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(STD)

slot_pool_test.o: slot_pool_test.cpp include/slot_pool.h
	$(CC) $(CFLAGS) -o $@ -c slot_pool_test.cpp $(STD)

slot_pool_test: slot_pool_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(STD)

clean:
	$(RM) *.o *~ *_test

# Install
$(INSTALL_DIR)/core:
	mkdir $@
	mkdir $@/include

$(INSTALL_DIR)/core/include/buffers.h: include/buffers.h
	cp $< $@

$(INSTALL_DIR)/core/include/slot_pool.h: include/slot_pool.h
	cp $< $@

$(INSTALL_DIR)/core/include/connection.h : include/connection.h
	cp $< $@

$(INSTALL_DIR)/core/include/connection_pool_interface.h: include/connection_pool_interface.h
	cp $< $@
