# Makefile Variables
INSTALL_FILES = $(INSTALL_DIR)/consul/include/factory_consul.h	$(INSTALL_DIR)/consul/include/consul_admin.h $(INSTALL_DIR)/consul/include/consul_interface.h $(INSTALL_DIR)/consul/include/service.h
CONSUL_INSTALL_DIR = $(INSTALL_DIR)/consul
OBJS = consul_admin.o service.o
INCL_DIRS = -I$(CURDIR)/../../

#Makefile Targets
default: $(OBJS)

tests: consul_test

benchmark: consul_benchmark

install: $(CONSUL_INSTALL_DIR) $(INSTALL_FILES)

clean:
	$(RM) *.o *~ *_test *_benchmark

# Makefile Instructions
# Create the object file consul_admin.o
consul_admin.o:  consul_admin.cpp include/consul_admin.h include/consul_interface.h
	$(CC) $(CFLAGS) -o $@ -c consul_admin.cpp $(STD) $(INCL_DIRS)

service.o: service.cpp include/service.h include/consul_interface.h
	$(CC) $(CFLAGS) -o $@ -c service.cpp $(STD) $(INCL_DIRS)

# Install
$(CONSUL_INSTALL_DIR):
	mkdir $@
	mkdir $@/include

$(CONSUL_INSTALL_DIR)/include/factory_consul.h: include/factory_consul.h
	cp $< $@

$(CONSUL_INSTALL_DIR)/include/consul_admin.h: include/consul_admin.h
	cp $< $@

$(CONSUL_INSTALL_DIR)/include/consul_interface.h: include/consul_interface.h
	cp $< $@

$(CONSUL_INSTALL_DIR)/include/service.h: include/service.h
	cp $< $@

# Test
# Create the executable file zmqio_test
consul_test: consul_test.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD) $(INCL_DIRS)

# Create the object file zmqio_test.o
consul_test.o: consul_test.cpp
	$(CC) $(CFLAGS) -o $@ -c consul_test.cpp $(STD) $(INCL_DIRS)

# Benchmark
consul_benchmark: consul_benchmark.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD) $(INCL_DIRS)

consul_benchmark.o: consul_benchmark.cpp
	$(CC) $(CFLAGS) -o $@ -c $^ $(STD) $(INCL_DIRS)
