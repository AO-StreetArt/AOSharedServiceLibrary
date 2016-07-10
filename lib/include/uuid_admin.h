//An admin for generating UUID's

#ifndef UUID_ADMIN
#define UUID_ADMIN

#include <string>
#include <string.h>
#include <uuid/uuid.h>
#include "logging.h"

class uuidAdmin
{
public:
	uuidAdmin();
	~uuidAdmin();
	std::string generate();
}

#endif
