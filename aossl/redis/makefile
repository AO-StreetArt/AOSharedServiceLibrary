# Makefile Variables
CC = g++
CFLAGS  = -g -Wall
STD = -std=c++11
INCL = $(INSTALL_DIR)/redis/include/factory_redis.h	$(INSTALL_DIR)/redis/include/redis_admin.h $(INSTALL_DIR)/redis/include/redis_interface.h
INCL_DIR = $(INSTALL_DIR)/redis
LIBS = `pkg-config --cflags --libs hiredis`
LIBS_RHEL = -lhiredis
RM = rm -f

#Makefile Targets
default: redis_admin.o

tests: redis_test

rhel-test: redis_test_rhel

benchmark: redis_benchmark

rhel-benchmark: redis_benchmark_rhel

install: $(INCL_DIR) $(INCL)

clean:
	$(RM) *.o *~ *_test *_benchmark

# Makefile Instructions
# Create the object file redis_admin.o
redis_admin.o:  redis_admin.cpp include/redis_admin.h include/redis_interface.h
	$(CC) $(CFLAGS) -o $@ -c redis_admin.cpp $(STD)

# Install
$(INSTALL_DIR)/redis:
	mkdir $@
	mkdir $@/include

$(INSTALL_DIR)/redis/include/factory_redis.h: include/factory_redis.h
	cp $< $@

$(INSTALL_DIR)/redis/include/redis_admin.h: include/redis_admin.h
	cp $< $@

$(INSTALL_DIR)/redis/include/redis_interface.h: include/redis_interface.h
	cp $< $@

# Test
# Create the executable file zmqio_test
redis_test: redis_admin.o redis_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD)

redis_test_rhel: redis_admin.o redis_test.o
	$(CC) $(CFLAGS) -o redis_test $^ $(LIBS_RHEL) $(STD)

# Create the object file zmqio_test.o
redis_test.o: redis_tests.cpp
	$(CC) $(CFLAGS) -o $@ -c redis_tests.cpp $(STD)

# Benchmark
redis_benchmark: redis_benchmark.o redis_admin.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD)

redis_benchmark_rhel: redis_benchmark.o redis_admin.o
	$(CC) $(CFLAGS) -o redis_benchmark $^ $(LIBS_RHEL) $(STD)

redis_benchmark.o: redis_benchmark.cpp
	$(CC) $(CFLAGS) -o $@ -c $^ $(STD)
