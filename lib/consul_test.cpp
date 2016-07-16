//Consul Admin Tests

#include "include/logging.h"
#include "include/consul_admin.h"
#include <string>
#include <string.h>
#include <assert.h>

int main()
{
//Construction tests
ConsulAdmin ca ("http://localhost:8500/");

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

logging->debug(ca.services_dc(""));

//Test Service Deregistration
ca.deregister_service(s);

logging->debug(ca.services_dc(""));

//Test Queries

logging->debug(ca.datacenters());
logging->debug(ca.nodes_dc(""));
logging->debug(ca.nodes_service("CLyman"));
logging->debug(ca.services_node(""));

//Test Key-Value Store
bool success = ca.set_config_value("Test", "123");
assert(success);

std::string test_val = ca.get_config_value("Test");
assert(test_val == "123");

success = ca.del_config_value("Test");
assert(success);

}
