//This is the Couchbase DB Admin
//This implements the DB Admin interface

//It requires you to implement
//Additional callback functions using the
//universal callbacks, which are called upon
//completion of the asynchronous threads

#include "factory/db_admin.h"
#include "factory/writeable.h"
#include "factory/couchbase_interface.h"

#include <sstream>
#include <string>
#include <string.h>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <exception>

extern "C"
{
	#include <libcouchbase/couchbase.h>
}

#ifndef COUCHBASE_ADMIN
#define COUCHBASE_ADMIN

extern CallbackInterface storage;
extern CallbackInterface retrieval;
extern CallbackInterface deletion;

//Transfer the callback information into a Request and call the registered callback
static void storage_callback(lcb_t instance, const void *cookie, lcb_storage_t op,
   lcb_error_t err, const lcb_store_resp_t *resp)
{
	//Build the request
	Request *r = new Request();
  RequestError *rerr = r->req_err;

	//Get the Key
	char *key_data = (char*)resp->v.v0.key;

	//Store the key in the request
	if (key_data)
	{
		std::string key (key_data);
		r->req_addr = key;
	}

	//Retrieve any errors
  if (err != LCB_SUCCESS) {
    rerr->err_code = COUCHBASE_BADREQUEST;
		rerr->err_message = lcb_strerror(instance, err);
  }
  else {
    rerr->err_code = NOERROR;
  }

	//Call the registered callback function
	std::string rresp = (*storage)(r);
	delete r;
}

static void get_callback(lcb_t instance, const void *cookie, lcb_error_t err,
   const lcb_get_resp_t *resp)
{
	//Build the request
	Request *r = new Request();
  RequestError *rerr = r->req_err;

	//Get the Key
	char *key_data = (char*)resp->v.v0.key;
	if (key_data)
	{
		std::string key (key_data);
		r->req_addr = key;
	}

	//Get the retrieved value
	char *data = (char*)resp->v.v0.bytes;
	if (data)
	{
		std::string val (data);
		r->req_data = val;
	}

	//Retrieve any errors
  if (err != LCB_SUCCESS) {
    rerr->err_code = COUCHBASE_BADREQUEST;
		rerr->err_message = lcb_strerror(instance, err);
  }
  else {
    rerr->err_code = NOERROR;
  }

	//Call the registered callback function
	std::string rresp = (*retrieval)(r);
	delete r;
}

static void del_callback(lcb_t instance, const void *cookie, lcb_error_t err, const lcb_remove_resp_t *resp)
{
	//Build the request
	Request *r = new Request();
  RequestError *rerr = r->req_err;

	//Get the Key
	char *key_data = (char*)resp->v.v0.key;

	//Store the key in the request
	if (key_data) {
		std::string key (key_data);
		r->req_addr = key;
	}

	//Retrieve any errors
  if (err != LCB_SUCCESS) {
    rerr->err_code = COUCHBASE_BADREQUEST;
		rerr->err_message = lcb_strerror(instance, err);
  }
  else {
		rerr->err_code = NOERROR;
  }

	//Call the registered callback function
	std::string rresp = (*deletion)(r);
	delete r;
}

//! The Couchbase Administrator handles interactions with the Couchbase DB

//! This binds a number of callbacks for major operations, and supports full CRUD operations
//! on JSON documents stored in the Couchbase DB
class CouchbaseAdmin: public CouchbaseInterface
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
	void load_object ( std::string key );

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
	void delete_object ( std::string key );

	//Bind Callbacks

	//! Bind the Retrieval Callback

	//! When the requested object is loaded, the method bound with
	//! bind_get_callback will be executed
	void bind_get_callback(CallbackInterface);

	//! Bind the Storage Callback

	//! When the requested object is saved or created, the method bound with
	//! bind_storage_callback will be executed
	void bind_storage_callback(CallbackInterface);

	//! Bind the Removal Callback

	//! When the requested object is deleted, the method bound with
	//! bind_delete_callback will be executed
	void bind_delete_callback(CallbackInterface);

	//! Get the instance, for advanced operations if necessary.  Not advised
	lcb_t get_instance ();

	//! Blocking call until the transaction stack is empty
	void wait ();
};

#endif
