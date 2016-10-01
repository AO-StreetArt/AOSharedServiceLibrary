#include "include/uuid_admin.h"

//Constructor & Destructor
//uuidAdmin::uuidAdmin() {}
//uuidAdmin::~uuidAdmin() {}

//Generate a new UUID
std::string uuidAdmin::generate()
{
int uuid_gen_result = 0;
uuid_t uuid;
uuid_gen_result = uuid_generate_time_safe(uuid);

if (uuid_gen_result == -1) {
	throw UuidSecurityException();
}

char uuid_str[37];
uuid_unparse_lower(uuid, uuid_str);

std::string str (uuid_str);
return str;
}
