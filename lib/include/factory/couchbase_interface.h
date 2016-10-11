//This is the Couchbase DB Admin
//This implements the DB Admin interface

//However, it also requires you to implement
//Additional callback functions unique to
//the couchbase engine, which are called upon
//completion of the asynchronous threads

#include "writeable.h"
#include "db_admin.h"
#include "callbacks.h"

#include <string>
#include <string.h>
#include <cstring>
#include <exception>

#include <libcouchbase/couchbase.h>


#ifndef COUCHBASE_INTERFACE
#define COUCHBASE_INTERFACE

struct CouchbaseInitException: public std::exception
{
	std::string int_msg;
  CouchbaseInitException (std::string msg) {int_msg = msg;}
  CouchbaseInitException () {}
  ~CouchbaseInitException() throw () {}
  const char * what() const throw ()
  {
		std::string what_str = "Couldn't create Couchbase instance: " + int_msg;
    return what_str.c_str();
  }
};

struct CouchbaseConnectException: public std::exception
{
	std::string int_msg;
  CouchbaseConnectException (std::string msg) {int_msg = msg;}
  CouchbaseConnectException () {}
  ~CouchbaseConnectException() throw () {}
  const char * what() const throw ()
  {
		std::string what_str = "Couldn't schedule Couchbase connection: " + int_msg;
    return what_str.c_str();
  }
};

struct CouchbaseBootstrapException: public std::exception
{
	std::string int_msg;
  CouchbaseBootstrapException (std::string msg) {int_msg = msg;}
  CouchbaseBootstrapException () {}
  ~CouchbaseBootstrapException() throw () {}
  const char * what() const throw ()
  {
		std::string what_str = "Couchbase Bootstrapping Failed: " + int_msg;
    return what_str.c_str();
  }
};

struct CouchbaseOperationException: public std::exception
{
	std::string int_msg;
  CouchbaseOperationException (std::string msg) {int_msg = msg;}
  CouchbaseOperationException () {}
  ~CouchbaseOperationException() throw () {}
  const char * what() const throw ()
  {
		std::string what_str = "Couldn't Schedule Couchbase Operation: " + int_msg;
    return what_str.c_str();
  }
};

//! The Couchbase Administrator handles interactions with the Couchbase DB

//! This binds a number of callbacks for major operations, and supports full CRUD operations
//! on JSON documents stored in the Couchbase DB
class CouchbaseInterface: public DBAdmin {
public:

	virtual ~CouchbaseInterface() {}

	//Object CRUD Operations

	//! Load a JSON Object from the Couchbase DB

	//! The requested object is loaded and, when ready,
	//! the method bound with bind_get_callback will be executed
	virtual void load_object ( const char * key ) = 0;

	//! The requested object is loaded and, when ready,
	//! the method bound with bind_get_callback will be executed
	virtual void load_object ( std::string key ) = 0;

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

	//! The requested object is deleted and, when complete,
	//! the method bound with bind_delete_callback will be executed
	virtual void delete_object ( std::string key ) = 0;

	//Bind Callbacks

	//! Bind the Retrieval Callback

	//! When the requested object is loaded, the method bound with
	//! bind_get_callback will be executed
	virtual void bind_get_callback(CallbackInterface) = 0;

	//! Bind the Storage Callback

	//! When the requested object is saved or created, the method bound with
	//! bind_storage_callback will be executed
	virtual void bind_storage_callback(CallbackInterface) = 0;

	//! Bind the Removal Callback

	//! When the requested object is deleted, the method bound with
	//! bind_delete_callback will be executed
	virtual void bind_delete_callback(CallbackInterface) = 0;

	//! Blocking call until the transaction stack is empty
	virtual void wait () = 0;
};

#endif
