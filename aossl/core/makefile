# Makefile Variables
CC = g++
CFLAGS  = -g -Wall
STD = -std=c++11
RM = rm -f
CORE_INSTALL_DIR = $(INSTALL_DIR)/core/include
INCL = $(CORE_INSTALL_DIR)/buffers.h $(CORE_INSTALL_DIR)/kv_store.h $(CORE_INSTALL_DIR)/kv_store_interface.h
INCL_DIR = $(INSTALL_DIR)/core

# Central Targets
tests: buffer_test

install: $(INCL_DIR) $(INCL)

# Tests
buffer_test.o: buffer_test.cpp include/buffers.h
	$(CC) $(CFLAGS) -o $@ -c buffer_test.cpp $(STD)

buffer_test: buffer_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(STD)

clean:
	$(RM) *.o *~ *_test

# Install
$(INSTALL_DIR)/core:
	mkdir $@
	mkdir $@/include

$(CORE_INSTALL_DIR)/buffers.h: include/buffers.h
	cp $< $@

$(CORE_INSTALL_DIR)/kv_store.h: include/kv_store.h
	cp $< $@

$(CORE_INSTALL_DIR)/kv_store_interface.h: include/kv_store_interface.h
	cp $< $@
