# Makefile Variables
SESSION_INSTALL_DIR = $(INSTALL_DIR)/profile/include
INCL = $(SESSION_INSTALL_DIR)/safe_app_profile.h $(SESSION_INSTALL_DIR)/tiered_app_profile.h $(SESSION_INSTALL_DIR)/app_profile.h $(SESSION_INSTALL_DIR)/network_app_profile.h
INCL_DIR = $(INSTALL_DIR)/profile
INCL_DIRS = -I$(CURDIR)/../../
INCL_OBJS = $(CURDIR)/../uuid/uuid_admin.o $(CURDIR)/../consul/consul_admin.o $(CURDIR)/../consul/service.o $(CURDIR)/../commandline/cli.o

# Central Targets
tests: profile_test

install: $(INCL_DIR) $(INCL)

# Tests
profile_test.o: profile_test.cpp
	$(CC) $(CFLAGS) -o $@ -c profile_test.cpp $(STD) $(INCL_DIRS)

profile_test: profile_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(STD) $(INCL_DIRS) $(INCL_OBJS) $(LIBS)

clean:
	$(RM) *.o *~ *_test

# Install
$(INSTALL_DIR)/profile:
	mkdir $@
	mkdir $@/include

$(SESSION_INSTALL_DIR)/network_app_profile.h: include/network_app_profile.h
	cp $< $@

$(SESSION_INSTALL_DIR)/safe_app_profile.h: include/safe_app_profile.h
	cp $< $@

$(SESSION_INSTALL_DIR)/tiered_app_profile.h: include/tiered_app_profile.h
	cp $< $@

$(SESSION_INSTALL_DIR)/app_profile.h: include/app_profile.h
	cp $< $@
