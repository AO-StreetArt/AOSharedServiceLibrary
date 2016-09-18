#include "include/response.h"
#include "include/factory/writeable.h"
#include <iostream>
#include <assert.h>

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

int main()
{

ApplicationResponse response;
assert(response.get_error_code() == NO_ERROR);

response.set_object_id("12345");
response.set_transaction_id("56789");

assert(response.get_object_id() == "12345");
assert(response.get_transaction_id() == "56789");

response.set_error(DB_ERROR);

std::cout << response.to_json() << std::endl;

response.set_error(BAD_REQUEST_ERROR, "Invalid Message type");

std::cout << response.to_json() << std::endl;

//Create an object
std::string name = "TestObject";
TestData *obj_ptr = new TestData (1, 2);
obj_ptr->set_key(name);

std::cout << response.to_json(obj_ptr);

delete obj_ptr;

return 0;
}
