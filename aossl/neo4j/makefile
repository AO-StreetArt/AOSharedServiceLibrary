# Makefile Variables
CC = g++
CFLAGS  = -g -Wall
STD = -std=c++11
INCL_DIRS = -I$(CURDIR)/../../
INCL = $(INSTALL_DIR)/neo4j/include/factory_neo4j.h	$(INSTALL_DIR)/neo4j/include/neo4j_admin.h $(INSTALL_DIR)/neo4j/include/neo4j_interface.h $(INSTALL_DIR)/neo4j/include/neo4j_connection_pool.h $(INSTALL_DIR)/neo4j/include/neo4j_list.h $(INSTALL_DIR)/neo4j/include/neo4j_map.h $(INSTALL_DIR)/neo4j/include/neo4j_object.h $(INSTALL_DIR)/neo4j/include/neo4j_query_parameter.h $(INSTALL_DIR)/neo4j/include/neo4j_result_iterator.h $(INSTALL_DIR)/neo4j/include/neo4j_result_tree.h $(INSTALL_DIR)/neo4j/include/neo4j_utils.h
INCL_DIR = $(INSTALL_DIR)/neo4j
LIBS = -lneo4j-client -lssl -lcrypto -lm
RM = rm -f

#Makefile Targets
default: neo4j_admin.o

tests: neo4j_test

install: $(INCL_DIR) $(INCL)

clean:
	$(RM) *.o *~ *_test *_benchmark

# Makefile Instructions
# Create the object file neo4j_admin.o
neo4j_admin.o:  neo4j_admin.cpp include/neo4j_admin.h include/neo4j_interface.h
	$(CC) $(CFLAGS) -o $@ -c neo4j_admin.cpp $(STD) $(INCL_DIRS)

# Install
$(INSTALL_DIR)/neo4j:
	mkdir $@
	mkdir $@/include

$(INSTALL_DIR)/neo4j/include/factory_neo4j.h: include/factory_neo4j.h
	cp $< $@

$(INSTALL_DIR)/neo4j/include/neo4j_admin.h: include/neo4j_admin.h
	cp $< $@

$(INSTALL_DIR)/neo4j/include/neo4j_interface.h: include/neo4j_interface.h
	cp $< $@

$(INSTALL_DIR)/neo4j/include/neo4j_connection_pool.h: include/neo4j_connection_pool.h
	cp $< $@

$(INSTALL_DIR)/neo4j/include/neo4j_list.h: include/neo4j_list.h
	cp $< $@

$(INSTALL_DIR)/neo4j/include/neo4j_map.h: include/neo4j_map.h
	cp $< $@

$(INSTALL_DIR)/neo4j/include/neo4j_object.h: include/neo4j_object.h
	cp $< $@

$(INSTALL_DIR)/neo4j/include/neo4j_query_parameter.h: include/neo4j_query_parameter.h
	cp $< $@

$(INSTALL_DIR)/neo4j/include/neo4j_result_iterator.h: include/neo4j_result_iterator.h
	cp $< $@

$(INSTALL_DIR)/neo4j/include/neo4j_result_tree.h: include/neo4j_result_tree.h
	cp $< $@

$(INSTALL_DIR)/neo4j/include/neo4j_utils.h: include/neo4j_utils.h
	cp $< $@

# Test
# Create the executable file zmqio_test
neo4j_test: neo4j_admin.o neo4j_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD) $(INCL_DIRS)

# Create the object file zmqio_test.o
neo4j_test.o: neo4j_test.cpp
	$(CC) $(CFLAGS) -o $@ -c neo4j_test.cpp $(STD) $(INCL_DIRS)
