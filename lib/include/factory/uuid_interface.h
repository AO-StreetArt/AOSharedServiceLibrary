//An admin for generating UUID's

#include <string>
#include <string.h>
#include <exception>

#ifndef UUID_INTERFACE
#define UUID_INTERFACE

//! A return structure which captures any security error messages thrown by the framework
struct UuidContainer {

	//! The UUID generated
	std::string id;

	//! Is either empty or contains an error message
	std::string err;
};

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
	virtual UuidContainer generate() = 0;
};

#endif
