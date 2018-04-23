# Simple makefile to allow for building & installation of all subfolders

make_subfolders:
	cd http && $(MAKE)
	cd consul && $(MAKE)
	cd commandline && $(MAKE)
	cd properties && $(MAKE)
	cd logging && $(MAKE)
	cd uuid && $(MAKE)

install:
	cd core && $(MAKE) install
	cd http && $(MAKE) install
	cd consul && $(MAKE) install
	cd commandline && $(MAKE) install
	cd properties && $(MAKE) install
	cd logging && $(MAKE) install
	cd uuid && $(MAKE) install

tests:
	cd core && $(MAKE) tests
	cd http && $(MAKE) tests
	cd commandline && $(MAKE) tests
	cd properties && $(MAKE) tests
	cd logging && $(MAKE) tests
	cd uuid && $(MAKE) tests
	cd consul && $(MAKE) tests

benchmarks:
	cd http && $(MAKE) benchmark
	cd consul && $(MAKE) benchmark
	cd logging && $(MAKE) benchmark
	cd uuid && $(MAKE) benchmark

clean:
	cd core && $(MAKE) clean
	cd http && $(MAKE) clean
	cd consul && $(MAKE) clean
	cd commandline && $(MAKE) clean
	cd properties && $(MAKE) clean
	cd logging && $(MAKE) clean
	cd uuid && $(MAKE) clean
