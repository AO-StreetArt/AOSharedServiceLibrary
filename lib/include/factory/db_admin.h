//This is the DB Admin Interface
//This defines the methods that must
//Be implemented in order to add support
//for a particular DB

#include "writeable.h"

#ifndef DB_ADMIN
#define DB_ADMIN

//! Database Administrator Interface

//! An Interface defining the behavior of different Document-based
//! Data Base Administrators
class DBAdmin
{
public:
	virtual ~DBAdmin() {}
	//Basic CRUD Operations

	//! Load a JSON Object from the DB
	virtual void load_object ( const char * key ) = 0;

	//! Save a JSON Object to the DB
	virtual void save_object ( Writeable *obj ) = 0;

	//! Create a JSON Object in the DB
	virtual void create_object (Writeable *obj ) = 0;

	//! Delete a JSON Object from the DB
	virtual void delete_object (const char * key) = 0;

	//! If the engine is asynchronous, wait for the
	//! active threads to complete.  Otherwise, do nothing
	virtual void wait () = 0;

	//Methods to prevent duplication of singleton
        //intentionally deleted
};

#endif
