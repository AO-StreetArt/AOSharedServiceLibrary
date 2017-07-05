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

#include <hayai/hayai.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <assert.h>

#include "include/consul_interface.h"
#include "include/factory_consul.h"

ConsulInterface *consul;
ServiceInterface *service;

std::vector<std::string> uuid_list;
int register_counter = 0;
int deregister_counter = 0;
int update_counter = 0;
int create_counter = 0;
int delete_counter = 0;

// Benchmarks

// Service Registration
BENCHMARK(CONSUL, RegisterService, 10, 100) {
  // Set the new ID of the service
  std::string uuid_str = uuid_list[register_counter];
  service->set_id(uuid_str);

  // Register the service
  consul->register_service(*service);

  // Update the register counter
  register_counter = register_counter + 1;
}

// Service Deregistration
BENCHMARK(CONSUL, DeregisterService, 10, 100) {
  // Set the new ID of the service
  std::string uuid_str = uuid_list[deregister_counter];
  service->set_id(uuid_str);

  // Register the service
  consul->deregister_service(*service);

  // Update the register counter
  deregister_counter = deregister_counter + 1;
}

// Configuration Value Retrieval
BENCHMARK(CONSUL, GetConfigurationValue, 10, 100) {
  std::string test_val = consul->get_config_value("Test");
  std::cout << test_val << std::endl;
}

// Configuration Value Update
BENCHMARK(CONSUL, UpdateConfigurationValue, 10, 100) {
  std::string uuid_str = uuid_list[update_counter];
  bool success = consul->set_config_value("Test", uuid_str);

  update_counter = update_counter + 1;
}

// Configuration Value Create
BENCHMARK(CONSUL, CreateConfigurationValue, 10, 100) {
  std::string uuid_str = uuid_list[create_counter];
  bool success = consul->set_config_value(uuid_str, "Test");

  create_counter = create_counter + 1;
}

// Configuration Value Delete
BENCHMARK(CONSUL, DeleteConfigurationValue, 10, 100) {
  std::string uuid_str = uuid_list[delete_counter];
  bool success = consul->del_config_value(uuid_str);

  delete_counter = delete_counter + 1;
}

// Main Method

int main() {
  ConsulComponentFactory consul_factory;

  // Set up UUID Generator
  consul = consul_factory.get_consul_interface("localhost:8500");

  service = \
    consul_factory.get_service_interface("1", "CLyman", "tcp://*", "5555");
  service->add_tag("Testing");

  // Generate the UUID's for the benchmarks
  int i = 0;
  for (i = 0; i < 1001; i++) {
    // Generate a new key for the object
    std::string uuid_str = std::to_string(i);
    uuid_list.push_back(uuid_str);
  }

  // Test Key-Value Store
  bool success = consul->set_config_value("Test", "123");
  assert(success);

  // Run Tests

  hayai::ConsoleOutputter consoleOutputter;

  hayai::Benchmarker::AddOutputter(consoleOutputter);
  hayai::Benchmarker::RunAllTests();

  // Post-Test Teardown

  delete consul;
  delete service;

  return 0;
}
