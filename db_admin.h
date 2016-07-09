//This is the DB Admin Interface
//This defines the methods that must
//Be implemented in order to add support
//for a particular DB

#include "writeable.h"

#ifndef DB_ADMIN
#define DB_ADMIN

class DBAdmin
{
public:
	DBAdmin() {}
	~DBAdmin() {}
	//Basic CRUD Operations
	virtual void load_object ( const char * key ) = 0;
	virtual void save_object ( Writeable const *obj ) = 0;
	virtual void create_object (Writeable const *obj ) = 0;
	virtual void delete_object (const char * key) = 0;

	//If the engine is asynchronous, wait for the
	//active threads to complete.  Otherwise, do nothing
	virtual void wait () = 0;

	//Methods to prevent duplication of singleton
        //intentionally deleted
};

#endif
