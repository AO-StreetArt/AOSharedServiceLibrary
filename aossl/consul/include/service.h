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

#ifndef AOSSL_CONSUL_INCLUDE_SERVICE_H_
#define AOSSL_CONSUL_INCLUDE_SERVICE_H_

#include <string.h>
#include <string>
#include <vector>
#include "consul_interface.h"

//! A Service class which can be registered with Consul for each app instance

//! An instance of this class can be instantiated by a service and is
//! passed to the consul admin to register and de-register
class Service: public ServiceInterface {
  std::string id;
  std::string name;
  std::vector<std::string> tags;
  std::string address;
  std::string port;
  HealthCheck check;

 public:
  //! Construct a Service
  Service();

  //! Construct a Service
  Service(std::string new_id, std::string new_name);

  //! Construct a Service
  Service(std::string new_id, std::string new_name, std::string new_address, \
     std::string new_port);

  //! Construct a Service
  Service(std::string new_id, std::string new_name, std::string new_address, \
     std::string new_port, std::vector<std::string> new_tags);

  //! Convert the Service into a JSON Message

  //! Method that allows the service to be transformed into a json message
  //! that can be sent via HTTP to a Consul instance
  std::string to_json() const;

  //! Get the Service ID
  std::string get_id() const {return id;}

  //! Get the Service Name
  std::string get_name() const {return name;}

  //! Get the Service Address
  std::string get_address() const {return address;}

  //! Get the Service Port
  std::string get_port() const {return port;}

  //! Set the Service ID
  void set_id(std::string new_id) {id = new_id;}

  //! Set the Service Name
  void set_name(std::string new_name) {name = new_name;}

  //! Set the Service Address
  void set_address(std::string new_address) {address = new_address;}

  //! Set the Service Port
  void set_port(std::string new_port) {port = new_port;}

  //! Get the tags
  std::vector<std::string> get_tags() {return tags;}
  //! Add a tag
  void add_tag(std::string new_tag) {tags.push_back(new_tag);}
  //! Clear the tags
  void clear_tags() {tags.clear();}
  //! How many tags are there?
  int num_tags() const {return tags.size();}

  //! Get the health checks
  HealthCheck get_check() {return check;}
  //! Add a check
  inline void set_check(std::string scr, int interval_seconds) {
    check.script = scr;
    check.interval = std::to_string(interval_seconds) + "s";
  }
};

#endif  // AOSSL_CONSUL_INCLUDE_SERVICE_H_
