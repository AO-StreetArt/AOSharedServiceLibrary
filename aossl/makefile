# Simple makefile to allow for building & installation of all subfolders

make_subfolders:
	cd consul && $(MAKE)
	cd commandline && $(MAKE)
	cd properties && $(MAKE)
	cd uuid && $(MAKE)

install:
	cd core && $(MAKE) install
	cd consul && $(MAKE) install
	cd commandline && $(MAKE) install
	cd properties && $(MAKE) install
	cd uuid && $(MAKE) install
	cd profile && $(MAKE) install

tests:
	cd core && $(MAKE) tests
	cd commandline && $(MAKE) tests
	cd properties && $(MAKE) tests
	cd uuid && $(MAKE) tests
	cd consul && $(MAKE) tests
	cd profile && $(MAKE) tests

benchmarks:
	cd consul && $(MAKE) benchmark
	cd uuid && $(MAKE) benchmark

clean:
	cd core && $(MAKE) clean
	cd consul && $(MAKE) clean
	cd commandline && $(MAKE) clean
	cd properties && $(MAKE) clean
	cd uuid && $(MAKE) clean
	cd profile && $(MAKE) clean
