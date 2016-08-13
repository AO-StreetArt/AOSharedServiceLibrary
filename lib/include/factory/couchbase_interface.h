//This is the Couchbase DB Admin
//This implements the DB Admin interface

//However, it also requires you to implement
//Additional callback functions unique to
//the couchbase engine, which are called upon
//completion of the asynchronous threads

#include "writeable.h"

#include <string>
#include <string.h>
#include <cstring>

extern "C"
{
	#include <libcouchbase/couchbase.h>
}

#ifndef COUCHBASE_INTERFACE
#define COUCHBASE_INTERFACE

//! Define the storage callback that will get passed to Couchbase
typedef void (*StorageCallback)(lcb_t, const void*, lcb_storage_t, lcb_error_t, const lcb_store_resp_t*);

//! Define the get callback that will get passed to Couchbase
typedef void (*GetCallback)(lcb_t, const void*, lcb_error_t, const lcb_get_resp_t*);

//! Define the delete callback that will get passed to Couchbase
typedef void (*DelCallback)(lcb_t, const void*, lcb_error_t, const lcb_remove_resp_t*);

//! The Couchbase Administrator handles interactions with the Couchbase DB

//! This binds a number of callbacks for major operations, and supports full CRUD operations
//! on JSON documents stored in the Couchbase DB
class CouchbaseInterface: public DBAdmin
{
public:

	//Object CRUD Operations

	//! Load a JSON Object from the Couchbase DB

	//! The requested object is loaded and, when ready,
	//! the method bound with bind_get_callback will be executed
	virtual void load_object ( const char * key ) = 0;

	//! Save a JSON Object to the Couchbase DB

	//! The requested object is saved and, when complete,
	//! the method bound with bind_storage_callback will be executed
	virtual void save_object ( Writeable *obj ) = 0;

	//! Create a JSON Object in the Couchbase DB

	//! The requested object is saved and, when complete,
	//! the method bound with bind_storage_callback will be executed
	virtual void create_object ( Writeable *obj ) = 0;

	//! Delete a JSON Object from the Couchbase DB

	//! The requested object is deleted and, when complete,
	//! the method bound with bind_delete_callback will be executed
	virtual void delete_object ( const char * key ) = 0;

	//Bind Callbacks

	//! Bind the Retrieval Callback

	//! When the requested object is loaded, the method bound with
	//! bind_get_callback will be executed
	virtual void bind_get_callback(GetCallback) = 0;

	//! Bind the Storage Callback

	//! When the requested object is saved or created, the method bound with
	//! bind_storage_callback will be executed
	virtual void bind_storage_callback(StorageCallback) = 0;

	//! Bind the Removal Callback

	//! When the requested object is deleted, the method bound with
	//! bind_delete_callback will be executed
	virtual void bind_delete_callback(DelCallback) = 0;

	//! Blocking call until the transaction stack is empty
	void wait () = 0;
};

#endif
