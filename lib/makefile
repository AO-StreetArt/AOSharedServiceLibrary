# Simple makefile to allow for building & installation of all subfolders

make_subfolders:
	cd http && $(MAKE)
	cd consul && $(MAKE)
	cd interpreter && $(MAKE)
	cd logging && $(MAKE)
	cd mongo && $(MAKE)
	cd neo4j && $(MAKE)
	cd redis && $(MAKE)
	cd uuid && $(MAKE)
	cd zmq && $(MAKE)

install:
	cd http && $(MAKE) install
	cd consul && $(MAKE) install
	cd interpreter && $(MAKE) install
	cd logging && $(MAKE) install
	cd mongo && $(MAKE) install
	cd neo4j && $(MAKE) install
	cd redis && $(MAKE) install
	cd uuid && $(MAKE) install
	cd zmq && $(MAKE) install

tests: base_test
	cd redis && $(MAKE) tests

rhel-test: base_test
	cd redis && $(MAKE) rhel-test

base_test:
	cd http && $(MAKE) tests
	cd interpreter && $(MAKE) tests
	cd logging && $(MAKE) tests
	cd mongo && $(MAKE) tests
	cd neo4j && $(MAKE) tests
	cd uuid && $(MAKE) tests
	cd zmq && $(MAKE) tests
	cd consul && $(MAKE) tests

benchmarks: base_benchmarks
	cd redis && $(MAKE) benchmark

rhel-benchmarks: base_benchmarks
	cd redis && $(MAKE) rhel-benchmark

base_benchmarks:
	cd http && $(MAKE) benchmark
	cd consul && $(MAKE) benchmark
	cd logging && $(MAKE) benchmark
	cd redis && $(MAKE) benchmark
	cd uuid && $(MAKE) benchmark
	cd zmq && $(MAKE) benchmark

clean:
	cd http && $(MAKE) clean
	cd consul && $(MAKE) clean
	cd interpreter && $(MAKE) clean
	cd logging && $(MAKE) clean
	cd mongo && $(MAKE) clean
	cd neo4j && $(MAKE) clean
	cd redis && $(MAKE) clean
	cd uuid && $(MAKE) clean
	cd zmq && $(MAKE) clean
