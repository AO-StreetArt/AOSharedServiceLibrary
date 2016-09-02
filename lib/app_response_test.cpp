#include "include/response.h"
#include <iostream>
#include <assert.h>

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

return 0;
}
