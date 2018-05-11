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

// Consul Admin Tests

#include <string.h>
#include <assert.h>
#include <string>
#include <iostream>
#include "include/factory_consul.h"
#include "include/consul_interface.h"
#include "aossl/core/include/buffers.h"

int main() {
  AOSSL::ConsulComponentFactory consul_factory;

  // Construction tests
  AOSSL::ConsulInterface *ca = \
    consul_factory.get_consul_interface("localhost:8500");

  AOSSL::ServiceInterface *s0 = \
    consul_factory.get_service_interface("0", "CLyman", "tcp://*", "5555");

  AOSSL::ServiceInterface *s = \
    consul_factory.get_service_interface("1", "CLyman", "tcp://*", "5555");
  s->add_tag("Testing");

  AOSSL::ServiceInterface *s2 = \
    consul_factory.get_service_interface("2", "CLyman", "tcp://*", "5557");
  s2->add_tag("Test");

  AOSSL::ServiceInterface *s3 = \
    consul_factory.get_service_interface("3", "OthrService", "tcp://*", "5559");
  s3->add_tag("Test");
  s3->clear_tags();
  s3->add_tag("Test2");
  s3->add_tag("Test3");
  assert(s3->num_tags() == 2);

  // Test Service Registration
  ca->register_service(*s);
  ca->register_service(*s0);

  // We need to figure out how to find nodes on the network
  // std::cout << ca->services_node(empty, empty) << std::endl;

  // Test Key-Value Store
  bool success = ca->set_config_value("Test", "123");
  assert(success);

  // This doesnt work since the return values are json messages and
  // the "Value" element is stored in base64 encoding
  // assert(test_val == "123");

  // Threadsafe tests
  std::string empty;
  std::string clyman = "Clyman";
  AOSSL::StringBuffer *buf0 = ca->services();
  AOSSL::StringBuffer *buf1 = ca->datacenters();
  AOSSL::StringBuffer *buf2 = ca->nodes_dc(empty);
  AOSSL::StringBuffer *buf3 = ca->nodes_service(clyman);
  AOSSL::StringBuffer *buf4 = ca->services_node(empty, empty);
  AOSSL::StringBuffer *buf5 = ca->get_opt("Test");

  std::cout << buf1->val << std::endl;
  std::cout << buf2->val << std::endl;
  std::cout << buf3->val << std::endl;
  std::cout << buf4->val << std::endl;
  std::cout << buf5->val << std::endl;
  std::cout << buf0->val << std::endl;

  assert(buf1->success);
  assert(buf2->success);
  assert(buf3->success);
  assert(buf4->success);
  assert(buf5->success);
  assert(buf0->success);

  delete buf0;
  delete buf1;
  delete buf2;
  delete buf3;
  delete buf4;
  delete buf5;

  // Cleanup tests
  success = ca->del_config_value("Test");
  assert(success);

  // Test Service Deregistration
  ca->deregister_service(*s);
  ca->deregister_service(*s0);

  delete ca;
  delete s;
  delete s0;
  delete s2;
  delete s3;
}
