
.PHONY: mk_helloworld mk_zmqshell

all: mk_helloworld mk_zmqshell mk_httpshell

mk_helloworld:
	@$(MAKE) -C hello_world

mk_zmqshell:
	@$(MAKE) -C zmq_shell

mk_httpshell:
	@$(MAKE) -C http_shell

clean:
	cd hello_world && $(MAKE) clean
	cd zmq_shell && $(MAKE) clean
	cd http_shell && $(MAKE) clean
