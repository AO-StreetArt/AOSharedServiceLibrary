# Makefile Variables
CORE_INSTALL_DIR = $(INSTALL_DIR)/vault/include
INCL = $(CORE_INSTALL_DIR)/vault_admin.h $(CORE_INSTALL_DIR)/factory_vault.h
INCL_DIR = $(INSTALL_DIR)/vault

# Central Targets
tests: vault_test

install: $(INCL_DIR) $(INCL)

# Tests
vault_test.o: vault_test.cpp include/vault_admin.h
	$(CC) $(CFLAGS) -o $@ -c vault_test.cpp $(STD)

vault_test: vault_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) $(STD)

# Clean
clean:
	$(RM) *.o *~ *_test

# Install
$(INSTALL_DIR)/vault:
	mkdir $@
	mkdir $@/include

$(CORE_INSTALL_DIR)/factory_vault.h: include/factory_vault.h
	cp $< $@

$(CORE_INSTALL_DIR)/vault_admin.h: include/vault_admin.h
	cp $< $@
