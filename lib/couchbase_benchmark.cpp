#include <hayai/hayai.hpp>
#include <uuid/uuid.h>
#include <libcouchbase/couchbase.h>

#include "include/factory/writeable.h"

#include "include/factory/couchbase_interface.h"
#include "include/factory/logging_interface.h"

#include "include/factory.h"

//Test Data Class with to_json method
class TestData: public Writeable
{
std::string key;
int i;
int j;
public:
  TestData(int a, int b) {i=a;j=b;}
  std::string get_key() {return key;}
  bool set_key(std::string new_key) {key=new_key; return true;}
  int get_i() {return i;}
  int get_j() {return j;}
  bool set_i(int new_i) {i = new_i; return true;}
  bool set_j(int new_j) {j = new_j; return true;}
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

TestData *obj;
CouchbaseInterface *cb;
std::vector<std::string> uuid_list;
int savecounter = 0;
int getcounter = 0;
int delcounter = 0;
int loadcounter = 0;

static void storage_callback(lcb_t instance, const void *cookie, lcb_storage_t op,
   lcb_error_t err, const lcb_store_resp_t *resp)
{
  if (err != LCB_SUCCESS) {
    fprintf(stderr, "Couldn't retrieve item: %s\n", lcb_strerror(instance, err));
  }
}

static void get_callback(lcb_t instance, const void *cookie, lcb_error_t err,
   const lcb_get_resp_t *resp)
{
  if (err != LCB_SUCCESS) {
    fprintf(stderr, "Couldn't retrieve item: %s\n", lcb_strerror(instance, err));
  }
}

static void del_callback(lcb_t instance, const void *cookie, lcb_error_t err, const lcb_remove_resp_t *resp)
{
  if (err != LCB_SUCCESS) {
    fprintf(stderr, "Couldn't retrieve item: %s\n", lcb_strerror(instance, err));
  }
}


//----------------------------------------------------------------------------//
//------------------------------Benchmarks------------------------------------//
//----------------------------------------------------------------------------//

BENCHMARK(Couchbase, Save, 10, 100)
{
  std::string uuid_str = uuid_list[savecounter];
  obj->set_key( uuid_str );

  cb->create_object ( obj );
  cb->wait();

  savecounter=savecounter+1;
}

BENCHMARK(Couchbase, Get, 10, 100)
{
  std::string uuid_str = uuid_list[getcounter];
  cb->load_object ( uuid_str.c_str() );
  cb->wait();

  getcounter=getcounter+1;
}

BENCHMARK(Couchbase, Update, 10, 100)
{
  std::string uuid_str = uuid_list[loadcounter];
  obj->set_key( uuid_str );

  cb->save_object ( obj );
  cb->wait();

  loadcounter=loadcounter+1;
}

BENCHMARK(Couchbase, Delete, 10, 100)
{
  std::string uuid_str = uuid_list[delcounter];
  cb->delete_object ( uuid_str.c_str() );
  cb->wait();

  delcounter=delcounter+1;
}

//----------------------------------------------------------------------------//
//------------------------------Main Method-----------------------------------//
//----------------------------------------------------------------------------//

int main()
{


ServiceComponentFactory factory;

//Application Setup

std::string initFileName = "src/test/log4cpp_test.properties";
logging = factory.get_logging_interface(initFileName);

//Generate the UUID's for the benchmarks
int i=0;
int uuid_gen_result;
uuid_t uuid;
for (i=0; i< 1001; i++) {
  //Generate a new key for the object
  std::string uuid_str = std::to_string(i);
  uuid_list.push_back(uuid_str);
}

//Generate an object to use for the benchmarks

obj = new TestData (0, 1);

//Set up the Couchbase Admin
//Build the Couchbase Admin (which will automatically connect to the DB)
cb = factory.get_couchbase_interface("couchbase://localhost/default");

//Supports both password authentication and clustering
printf("Connected to Couchbase");
//Bind callbacks
cb->bind_storage_callback(storage_callback);
cb->bind_get_callback(get_callback);
cb->bind_delete_callback(del_callback);

//------------------------------Run Tests-------------------------------------//
//----------------------------------------------------------------------------//

hayai::ConsoleOutputter consoleOutputter;

hayai::Benchmarker::AddOutputter(consoleOutputter);
hayai::Benchmarker::RunAllTests();

//-------------------------Post-Test Teardown---------------------------------//
//----------------------------------------------------------------------------//

delete obj;
delete cb;
delete logging;

return 0;

}
