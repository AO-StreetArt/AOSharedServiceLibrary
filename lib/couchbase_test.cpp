#include <libcouchbase/couchbase.h>
#include <string>
#include <string.h>
#include <iostream>

#include "include/couchbase_admin.h"

#include "include/factory/writeable.h"
#include "include/factory/callbacks.h"

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

//Couchbase Callbacks
std::string my_storage_callback (Request *r)
{
  if (r->req_err->err_code == NOERROR)
  {
    std::cout << "stored: " << r->req_addr << std::endl;
  }
  else
  {
    std::cout << "Failed to Store: " << r->req_addr << std::endl;
    std::cout << r->req_err->err_message << std::endl;
  }
  return r->req_addr;
}

std::string my_retrieval_callback (Request *r)
{
  if (r->req_err->err_code == NOERROR)
  {
    std::cout << "retrieved: " << r->req_addr << std::endl;
    std::cout << "value: " << r->req_data << std::endl;
  }
  else
  {
    std::cout << "Failed to Store: " << r->req_addr << std::endl;
    std::cout << r->req_err->err_message << std::endl;
  }
  return r->req_addr;
}

std::string my_delete_callback (Request *r)
{
  if (r->req_err->err_code == NOERROR)
  {
    std::cout << "removed: " << r->req_addr << std::endl;
  }
  else
  {
    std::cout << "Failed to Delete: " << r->req_addr << std::endl;
    std::cout << r->req_err->err_message << std::endl;
  }
  return r->req_addr;
}

int main ()
{

//Create an object
std::string name = "TestObject";
TestData *obj_ptr = new TestData (1, 2);
obj_ptr->set_key(name);
const char* obj_key = obj_ptr->get_key().c_str();

//Build the Couchbase Admin (which will automatically connect to the DB)
CouchbaseAdmin cb ("couchbase://localhost/default");

//Supports both password authentication and clustering
printf("Connected to Couchbase");
//Bind callbacks
cb.bind_get_callback(my_retrieval_callback);
cb.bind_storage_callback(my_storage_callback);
cb.bind_delete_callback(my_delete_callback);
printf("Callbacks bound");
//Write the object to the DB
cb.create_object ( obj_ptr );
cb.wait();
printf("Create Object Tested");
//Get the object from the DB
cb.load_object ( obj_key );
cb.wait();
printf("Load Object Tested");
//Update the object in the DB
obj_ptr->set_i ( 10 );
cb.save_object ( obj_ptr );
cb.wait();
printf("Save Object Tested");
//Get the object from the DB to ensure it updates correctly
cb.load_object ( obj_key );
cb.wait();
//Delete the object
cb.delete_object ( obj_key );
cb.wait();
printf("Delete Object Tested");

return 0;
}
