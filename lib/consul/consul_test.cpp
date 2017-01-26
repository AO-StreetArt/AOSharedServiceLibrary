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

//Consul Admin Tests

#include "include/factory_consul.h"
#include "include/factory/consul_interface.h"
#include <string>
#include <string.h>
#include <assert.h>
#include <iostream>

int main()
{

ConsulComponentFactory consul_factory;

//Construction tests
ConsulInterface *ca = consul_factory.get_consul_interface("localhost:8500");

ServiceInterface *s0 = consul_factory.get_service_interface("0", "CLyman", "tcp://*", "5555");

ServiceInterface *s = consul_factory.get_service_interface("1", "CLyman", "tcp://*", "5555");
s->add_tag("Testing");

ServiceInterface *s2 = consul_factory.get_service_interface("2", "CLyman", "tcp://*", "5557");
s2->add_tag("Test");

ServiceInterface *s3 = consul_factory.get_service_interface("3", "OtherService", "tcp://*", "5559");
s3->add_tag("Test");
s3->clear_tags();
s3->add_tag("Test2");
s3->add_tag("Test3");
assert(s3->num_tags() == 2);

//Test Service Registration
ca->register_service(*s);
ca->register_service(*s0);

std::cout << ca->services() << std::endl;

//Test Queries

std::string empty = "";
std::string clyman = "CLyman";
std::cout << ca->datacenters() << std::endl;
std::cout << ca->nodes_dc(empty) << std::endl;
std::cout << ca->nodes_service(clyman) << std::endl;

//We need to figure out how to find nodes on the network
std::cout << ca->services_node(empty, empty) << std::endl;

//Test Key-Value Store
bool success = ca->set_config_value("Test", "123");
assert(success);

std::string test_val = ca->get_config_value("Test");
std::cout << test_val << std::endl;
//This doesnt work since the return values are json messages and the "Value" element is stored in base64 encoding
//assert(test_val == "123");

success = ca->del_config_value("Test");
assert(success);

//Test Service Deregistration
ca->deregister_service(*s);
ca->deregister_service(*s0);

std::cout << ca->services() << std::endl;

delete ca;
delete s;
delete s0;
delete s2;
delete s3;

}
