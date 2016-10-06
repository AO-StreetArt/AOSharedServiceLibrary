#include "include/uuid_admin.h"

//Constructor & Destructor
//uuidAdmin::uuidAdmin() {}
//uuidAdmin::~uuidAdmin() {}

//Generate a new UUID
UuidContainer uuidAdmin::generate()
{
UuidContainer cont;
int uuid_gen_result = 0;
uuid_t uuid;
uuid_gen_result = uuid_generate_time_safe(uuid);

if (uuid_gen_result == -1) {
	cont.err = "UUID Generated in an unsafe manner that exposes a potential security risk : http://linux.die.net/man/3/uuid_generate";
}
else {
	cont.err = "";
}

char uuid_str[37];
uuid_unparse_lower(uuid, uuid_str);

std::string str (uuid_str);
cont.id = str;
return cont;
}
