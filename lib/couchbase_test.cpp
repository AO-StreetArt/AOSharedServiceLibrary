#include "include/couchbase_admin.h"
#include <libcouchbase/couchbase.h>
#include "include/writeable.h"
#include <string>
#include <string.h>

//Test Data Class with to_json method
class TestData: public Writeable
{
std::string key;
int i;
int j;
public:
  TestData(int a, int b) {i=a;j=b;}
  std::string get_key() {return key;}
  bool set_key(std::string new_key) {key=new_key;}
  std::string to_json()
  {
    std::string json_str = "{key:";
    std::string istr = std::to_string(i);
    std::string jstr = std::to_string(j);
    json_str = json_str.append(key);
    json_str = json_str.append(", i:");
    json_str = json_str.append(istr);
    json_str = json_str.append(", j:");
    json_str = json_str.append(jstr);
    json_str = json_str.append("}");
    return json_str;
  }
};

static void storage_callback(lcb_t instance, const void *cookie, lcb_storage_t op,
   lcb_error_t err, const lcb_store_resp_t *resp)
{
  if (err == LCB_SUCCESS) {
    printf("Stored %.*s\n", (int)resp->v.v0.nkey, (char*)resp->v.v0.key);
  }
  else {
    fprintf(stderr, "Couldn't retrieve item: %s\n", lcb_strerror(instance, err));
  }
}

static void get_callback(lcb_t instance, const void *cookie, lcb_error_t err,
   const lcb_get_resp_t *resp)
{
  printf("Retrieved key %.*s\n", (int)resp->v.v0.nkey, (char*)resp->v.v0.key);
  printf("Value is %.*s\n", (int)resp->v.v0.nbytes, (char*)resp->v.v0.bytes);
}

int main ()
{

std::string initFileName = "test/log4cpp.properties";
try {
	log4cpp::PropertyConfigurator::configure(initFileName);
}
catch ( log4cpp::ConfigureFailure &e ) {
	printf("[log4cpp::ConfigureFailure] caught while reading Logging Configuration File");
	printf(e.what());
	exit(1);
}

log4cpp::Category& root = log4cpp::Category::getRoot();

log4cpp::Category& sub1 = log4cpp::Category::getInstance(std::string("sub1"));

log4cpp::Category& log = log4cpp::Category::getInstance(std::string("sub1.log"));

logging = &log;

//Create an object
std::string name = "TestObject";
TestData data (1, 2);
data.set_key(name);
const char* obj_key = data.get_key().c_str();

//Build the Couchbase Admin (which will automatically connect to the DB)
CouchbaseAdmin cb ("couchbase://localhost/default");

//Supports both password authentication and clustering
printf("Connected to Couchbase");
//Bind callbacks
lcb_set_store_callback(cb.get_instance(), storage_callback);
lcb_set_get_callback(cb.get_instance(), get_callback);
printf("Callbacks bound");
//Write the object to the DB
TestData *obj_ptr = &data;
cb.create_object ( obj_ptr );
cb.wait();
printf("Create Object Tested");
//Get the object from the DB
cb.load_object ( obj_key );
cb.wait();
printf("Load Object Tested");
//Update the object in the DB
data.set_name ( "Weeee" );
cb.save_object ( obj_ptr );
cb.wait();
printf("Save Object Tested");
//Delete the object
cb.delete_object ( obj_key );
cb.wait();
printf("Delete Object Tested");

return 0;
}
