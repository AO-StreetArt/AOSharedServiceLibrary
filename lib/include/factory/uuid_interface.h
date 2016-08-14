//An admin for generating UUID's

#ifndef UUID_INTERFACE
#define UUID_INTERFACE

#include <string>
#include <string.h>

//! UUID Admin

//! The UUID Admin is in charge of generating any Universally Unique
//! ID's that are required throughout program execution
class uuidInterface
{
public:

	virtual ~uuidInterface() {}
	//! Generate a new UUID

	//! The method will generate on the means of generation present on your system
	//! In some cases, this may result in UUID's being generated that pose a security
	//! risk.  In this case, that fact will be clearly called out in the logs, and
	//! it is recommended that production systems are tested to ensure that UUID's are
	//! generated in a safe manner
	virtual std::string generate() = 0;
};

#endif
