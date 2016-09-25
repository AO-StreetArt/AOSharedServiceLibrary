//An admin for generating UUID's

#include <string>
#include <string.h>
#include <exception>

#ifndef UUID_INTERFACE
#define UUID_INTERFACE

struct UuidSecurityException: public std::exception
{
	const char * what() const throw ()
    {
      return "UUID Generated in an unsafe manner that exposes a potential security risk : http://linux.die.net/man/3/uuid_generate";
    }
}

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
