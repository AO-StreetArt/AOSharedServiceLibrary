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
	uuid_logging->error("UUID Generated in an unsafe manner that exposes a potential security risk");
	uuid_logging->error("http://linux.die.net/man/3/uuid_generate");
	uuid_logging->error("Please take the needed actions to allow uuid generation with a safe generator");
}

char uuid_str[37];
uuid_unparse_lower(uuid, uuid_str);

std::string str (uuid_str);
return str;
}
