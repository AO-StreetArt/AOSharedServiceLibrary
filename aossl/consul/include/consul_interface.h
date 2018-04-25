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

#include <string.h>
#include <string>
#include <vector>

#include "aossl/core/include/buffers.h"
#include "aossl/core/include/kv_store_interface.h"

#ifndef AOSSL_CONSUL_INCLUDE_CONSUL_INTERFACE_H_
#define AOSSL_CONSUL_INCLUDE_CONSUL_INTERFACE_H_

namespace AOSSL {

// ----------------------------Health Checks----------------------------------//

//! A struct to hold health check information which can be added to a service
struct HealthCheck {
  //! The script to run for the health check
  std::string script;

  //! The interval for the health check
  std::string interval;
};

// -------------------------------Service-------------------------------------//

//! A Service class which can be registered with Consul for each app instance

//! An instance of this class can be instantiated by a service and is
//! passed to the consul admin to register and de-register
class ServiceInterface {
 public:
  virtual ~ServiceInterface() {}
  //! Convert the Service into a JSON Message

  //! Method that allows the service to be transformed into a json message
  //! that can be sent via HTTP to a Consul instance
  virtual std::string to_json() const = 0;

  //! Get the Service ID
  virtual std::string get_id() const = 0;

  //! Get the Service Name
  virtual std::string get_name() const = 0;

  //! Get the Service Address
  virtual std::string get_address() const = 0;

  //! Get the Service Port
  virtual std::string get_port() const = 0;

  //! Set the Service ID
  virtual void set_id(std::string new_id) = 0;

  //! Set the Service Name
  virtual void set_name(std::string new_name) = 0;

  //! Set the Service Address
  virtual void set_address(std::string new_address) = 0;

  //! Set the Service Port
  virtual void set_port(std::string new_port) = 0;

  //! Get the tags
  virtual std::vector<std::string> get_tags() = 0;
  //! Add a tag
  virtual void add_tag(std::string new_tag) = 0;
  //! Clear the tags
  virtual void clear_tags() = 0;
  //! How many tags are there?
  virtual int num_tags() const = 0;

  //! Get the health checks
  virtual HealthCheck get_check() = 0;
  //! Add a check
  virtual void set_check(std::string scr, int interval_seconds) = 0;
};

// ----------------------------Consul Admin-----------------------------------//

//! The Consul Administrator, who handles configuration & service discovery

//! This relies on the HTTP Administrator, and takes in a Service object
//! in order to register.  It's responses are JSON strings that are recieved
//! from Consul.  Note that the values returned from the Key-Value store
//! will be stored in base64 format
class ConsulInterface: public KeyValueStoreInterface  {
 public:
  //! Convinience Method for base64 decoding

  //! This is needed as all configuration values are returned
  //! from Consul in base64, and need to be decoded after the json
  //! is parsed.
  /*!
    \param encoded_string The string to decode
  */
  virtual AOSSL::StringBuffer* \
    base64_decode_safe(std::string const& encoded_string) = 0;

  virtual ~ConsulInterface() {}

  // -----------------Service Registry Functions------------------------------//

  //! Register the Service
  /*!
    \param s The Service Interface to send to the Consul Agent
  */
  virtual bool register_service(const ServiceInterface& s) = 0;

  //! Deregister the Service
  /*!
    \param s The Service Interface to send to the Consul Agent
  */
  virtual bool deregister_service(const ServiceInterface& s) = 0;

  // ------------Configuration Key-Value Storage Functions--------------------//

  //! Set a configuration value.

  //! If the key does not exist, then this will add it.
  //! Otherwise, it will update the existing key.
  /*!
    \param key A string key that will be created/updated in the KV Store
    \param val A string value that will be stored in the KV Store
  */
  virtual bool set_config_value(std::string key, std::string val) = 0;

  //! Delete a configuration value
  virtual bool del_config_value(std::string key) = 0;

  // Basic Queries
  // All Return a JSON string

  // Local Agent Queries

  //! Query the local agent for services registered

  //! Query the local agent for services registered
  virtual AOSSL::StringBuffer* services() = 0;

  //! Query the local agent for it's info
  virtual AOSSL::StringBuffer* agent_info() = 0;

  //! Query for healthy services only
  virtual AOSSL::StringBuffer* healthy_services() = 0;

  //! Query the catalog for datacenters
  virtual AOSSL::StringBuffer* datacenters() = 0;

  //! Query the catalog for the nodes in a particular datacenter
  /*!
    \param data_center The string identifier for the data center to query
  */
  virtual AOSSL::StringBuffer* nodes_dc(std::string data_center) = 0;

  //! Query the catalog for the services in a particular datacenter
  /*!
    \param data_center The string identifier for the data center to query
  */
  virtual AOSSL::StringBuffer* services_dc(std::string data_center) = 0;

  //! Query the catalog for the nodes running a particular service
  /*!
    \param service The string identifier for the service to query
  */
  virtual AOSSL::StringBuffer* nodes_service(std::string service) = 0;

  //! Query the catalog for the services provided by a particular node
  /*!
    \param node The string identifier for the node to query
    \param data_center The string id for the data center which holds the node
  */
  virtual AOSSL::StringBuffer* \
    services_node(std::string node, std::string data_center) = 0;
};

}

#endif  // AOSSL_CONSUL_INCLUDE_CONSUL_INTERFACE_H_
