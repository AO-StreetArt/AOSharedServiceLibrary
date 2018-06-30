/*
MIT License Block

Copyright (c) 2016 Alex Barry

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <string>
#include <iostream>
#include <vector>

#include "aossl/core/include/buffers.h"
#include "aossl/core/include/kv_store_interface.h"

#include "aossl/consul/include/consul_interface.h"
#include "aossl/uuid/include/uuid_interface.h"

#include "aossl/commandline/include/factory_cli.h"
#include "aossl/properties/include/factory_props.h"
#include "aossl/consul/include/factory_consul.h"
#include "aossl/uuid/include/factory_uuid.h"

#include "aossl/vault/include/vault_interface.h"
#include "aossl/vault/include/factory_vault.h"

#ifndef AOSSL_PROFILE_INCLUDE_APP_PROFILE_H_
#define AOSSL_PROFILE_INCLUDE_APP_PROFILE_H_

namespace AOSSL {

//! Application Session which manages configuration elements.

//! Provides access to all of the config elements,
//! but does not have a KV Store
class ApplicationProfile {
  CommandLineInterpreterFactory cli_factory;
  PropertyReaderFactory props_factory;
  ConsulComponentFactory consul_factory;
  UuidComponentFactory uuid_factory;
  VaultComponentFactory vault_factory;
  VaultInterface *vault = nullptr;
  KeyValueStoreInterface *cli = NULL;
  KeyValueStoreInterface *cfile = NULL;
  ConsulInterface *consul = NULL;
  UuidInterface *uuid = NULL;
  std::string application_name;
  std::string profile_name;

 public:
  //! Create a new Application Session with Command Line Arguments
  ApplicationProfile(int argc, char* argv[]) {
    cli = cli_factory.get_command_line_interface(argc, argv);
    uuid = uuid_factory.get_uuid_interface();
  }

  //! Create a new Application Session with Command Line Arguments
  ApplicationProfile(int argc, char* argv[], std::string app_name, \
      std::string prof_name) {
    cli = cli_factory.get_command_line_interface(argc, argv);
    uuid = uuid_factory.get_uuid_interface();
    application_name.assign(app_name);
    profile_name.assign(prof_name);
  }

  //! Create a new Application Session with Command Line Arguments
  explicit ApplicationProfile(const std::vector<std::string>& args) {
    cli = cli_factory.get_command_line_interface(args);
    uuid = uuid_factory.get_uuid_interface();
  }

  //! Create a new Application Session with Command Line Arguments
  ApplicationProfile(const std::vector<std::string>& args, \
    std::string app_name, std::string prof_name) {
    cli = cli_factory.get_command_line_interface(args);
    uuid = uuid_factory.get_uuid_interface();
    application_name.assign(app_name);
    profile_name.assign(prof_name);
  }
  
  //! Create a new Application Session without any Command Line Arguments
  ApplicationProfile(std::string app_name, std::string prof_name) {
    uuid = uuid_factory.get_uuid_interface();
    application_name.assign(app_name);
    profile_name.assign(prof_name);
  }

  virtual inline ~ApplicationProfile() {
    if (cli) {delete cli;}
    if (cfile) {delete cfile;}
    if (consul) {delete consul;}
    if (uuid) {delete uuid;}
  }

  //! Set the location of the properties file
  inline void set_property_file(std::string& filename) {
    if (cfile) {
      delete cfile;
      cfile = NULL;
    }
    cfile = props_factory.get_properties_reader_interface(filename);
  }

  //! Set the address of the consul agent
  inline void set_consul_address(std::string caddr) {
    if (consul) {
      delete consul;
      consul = NULL;
    }
    consul = consul_factory.get_consul_interface(caddr);
  }

  //! Set the address of the consul agent
  inline void set_consul_address(std::string caddr, int tout, std::string ssl_cert) {
    if (consul) {
      delete consul;
      consul = NULL;
    }
    consul = consul_factory.get_consul_interface(caddr, tout, ssl_cert);
  }

  //! Set the address of the consul agent
  inline void set_consul_address(std::string caddr, int tout, std::string ssl_cert, std::string acl_token) {
    if (consul) {
      delete consul;
      consul = NULL;
    }
    consul = consul_factory.get_consul_interface(caddr, tout, ssl_cert, acl_token);
  }

  //! Update the Vault connectivity information
  inline void set_vault_address(std::string& vaddr, std::string& secrets_path, \
      int tout, std::string& cert, int auth_type, std::string& un, std::string& pw) {
    if (vault) {
      delete vault;
      vault = nullptr;
    }
    vault = vault_factory.get_vault_interface(vaddr, secrets_path, 5, cert, auth_type, un, pw);
  }

  //! Update the Vault connectivity information
  inline void set_vault_address(std::string& vaddr, std::string& secrets_path, \
      int tout, int auth_type, std::string& un, std::string& pw) {
    if (vault) {
      delete vault;
      vault = nullptr;
    }
    vault = vault_factory.get_vault_interface(vaddr, secrets_path, 5, auth_type, un, pw);
  }

  //! Get the Command Line Interface
  KeyValueStoreInterface* get_cli() {return cli;}

  //! Get the Properties File Reader
  KeyValueStoreInterface* get_props() {return cfile;}

  //! Get the Consul Interface
  ConsulInterface* get_consul() {return consul;}

  //! Get the Vault Interface
  VaultInterface* get_vault() {return vault;}

  //! Get the UUID Interface
  UuidInterface* get_uuid() {return uuid;}

  //! Get the Application Name
  std::string get_app_name() {return application_name;}

  //! Get the Profile Name
  std::string get_profile_name() {return profile_name;}

  //! Get the Application Name
  void set_app_name(std::string name) {application_name.assign(name);}

  //! Get the Profile Name
  void set_profile_name(std::string prof) {profile_name.assign(prof);}
};

}  // namespace AOSSL

#endif  // AOSSL_PROFILE_INCLUDE_APP_PROFILE_H_
