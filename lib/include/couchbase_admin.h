//This is the Couchbase DB Admin
//This implements the DB Admin interface

//However, it also requires you to implement
//Additional callback functions unique to
//the couchbase engine, which are called upon
//completion of the asynchronous threads

#include "db_admin.h"
#include "writeable.h"

#include <sstream>
#include <string>
#include <string.h>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <exception>

#include "logging.h"

extern "C"
{
	#include <libcouchbase/couchbase.h>
}

#ifndef COUCHBASE_ADMIN
#define COUCHBASE_ADMIN

//Define the callbacks that will get passed to Couchbase
typedef static void (*StorageCallback)(lcb_t, const void*, lcb_storage_t, lcb_error_t, const lcb_store_resp_t*);
typedef static void (*GetCallback)(lcb_t, const void*, lcb_error_t, const lcb_get_resp_t*);
typedef static void (*DelCallback)(lcb_t, const void*, lcb_error_t, const lcb_remove_resp_t*);

class CouchbaseAdmin: public DBAdmin
{
lcb_t private_instance;
bool authentication_active;
const char * password;
void initialize (const char * conn);
public:
	//Constructor & Destructor
	CouchbaseAdmin ( const char * conn );
	CouchbaseAdmin ( const char * conn, const char * pswd );
	~CouchbaseAdmin ();

	//Object CRUD Operations
	void load_object ( const char * key );
	void save_object ( Writeable *obj );
	void create_object ( Writeable *obj );
	void delete_object ( const char * key );

	//Bind Callbacks
	void bind_get_callback(GetCallback);
	void bind_storage_callback(StorageCallback);
	void bind_delete_callback(DelCallback);

	//Get the instance, for advanced operations if necessary
	lcb_t get_instance ();

	//Blocking call until the transaction stack is empty
	void wait ();
};

#endif
