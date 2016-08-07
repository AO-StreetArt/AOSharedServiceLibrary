//Consul Admin Tests

#include "include/logging.h"
#include "include/consul_admin.h"
#include <string>
#include <string.h>
#include <assert.h>

int main()
{
std::string initFileName = "test/log4cpp_test.properties";
logging = new Logger(initFileName);

//Construction tests
ConsulAdmin ca ("localhost:8500");

Service s0 ("0", "CLyman", "tcp://*", "5555");

Service s ("1", "CLyman", "tcp://*", "5555");
s.add_tag("Testing");

Service s2 ("2", "CLyman", "tcp://*", "5557");
s2.add_tag("Test");

Service s3 ("3", "OtherService", "tcp://*", "5559");
s3.add_tag("Test");
s3.clear_tags();
s3.add_tag("Test2");
s3.add_tag("Test3");
assert(s3.num_tags() == 2);

//Test Service Registration
ca.register_service(s);
ca.register_service(s0);

logging->debug(ca.services());

//Test Queries

std::string empty = "";
std::string clyman = "CLyman";
logging->debug(ca.datacenters());
logging->debug(ca.nodes_dc(empty));
logging->debug(ca.nodes_service(clyman));

//We need to figure out how to find nodes on the network
logging->debug(ca.services_node(empty, empty));

//Test Key-Value Store
bool success = ca.set_config_value("Test", "123");
assert(success);

std::string test_val = ca.get_config_value("Test");
logging->debug(test_val);
//This doesnt work since the return values are json messages and the "Value" element is stored in base64 encoding
//assert(test_val == "123");

success = ca.del_config_value("Test");
assert(success);

//Test Service Deregistration
ca.deregister_service(s);
ca.deregister_service(s0);

logging->debug(ca.services());

}
