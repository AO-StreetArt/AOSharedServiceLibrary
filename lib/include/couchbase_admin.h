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
typedef void (*StorageCallback)(lcb_t, const void*, lcb_storage_t, lcb_error_t, const lcb_store_resp_t*);
typedef void (*GetCallback)(lcb_t, const void*, lcb_error_t, const lcb_get_resp_t*);
typedef void (*DelCallback)(lcb_t, const void*, lcb_error_t, const lcb_remove_resp_t*);

//! The Couchbase Administrator handles interactions with the Couchbase DB

//! This binds a number of callbacks for major operations, and supports full CRUD operations
//! on JSON documents stored in the Couchbase DB
class CouchbaseAdmin: public DBAdmin
{
lcb_t private_instance;
bool authentication_active;
const char * password;
void initialize (const char * conn);
public:
	//! Create a new Couchbase Admin, without a password
	CouchbaseAdmin ( const char * conn );

	//! Create a new Couchbase Admin, with a password
	CouchbaseAdmin ( const char * conn, const char * pswd );

	//! Delete the Couchbase Admin
	~CouchbaseAdmin ();

	//Object CRUD Operations

	//! Load a JSON Object from the Couchbase DB

	//! The requested object is loaded and, when ready,
	//! the method bound with bind_get_callback will be executed
	void load_object ( const char * key );

	//! Save a JSON Object to the Couchbase DB

	//! The requested object is saved and, when complete,
	//! the method bound with bind_storage_callback will be executed
	void save_object ( Writeable *obj );

	//! Create a JSON Object in the Couchbase DB

	//! The requested object is saved and, when complete,
	//! the method bound with bind_storage_callback will be executed
	void create_object ( Writeable *obj );

	//! Delete a JSON Object from the Couchbase DB

	//! The requested object is deleted and, when complete,
	//! the method bound with bind_delete_callback will be executed
	void delete_object ( const char * key );

	//Bind Callbacks

	//! Bind the Retrieval Callback

	//! When the requested object is loaded, the method bound with
	//! bind_get_callback will be executed
	void bind_get_callback(GetCallback);

	//! Bind the Storage Callback

	//! When the requested object is saved or created, the method bound with
	//! bind_storage_callback will be executed
	void bind_storage_callback(StorageCallback);

	//! Bind the Removal Callback

	//! When the requested object is deleted, the method bound with
	//! bind_delete_callback will be executed
	void bind_delete_callback(DelCallback);

	//! Get the instance, for advanced operations if necessary.  Not advised
	lcb_t get_instance ();

	//! Blocking call until the transaction stack is empty
	void wait ();
};

#endif
