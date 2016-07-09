//This is the Couchbase DB Admin
//This implements the DB Admin interface

//However, it also requires you to implement
//Additional callback functions unique to
//the couchbase engine, which are called upon
//completion of the asynchronous threads

#include "db_admin.h"

#include <sstream>
#include <string>
#include <string.h>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <exception>

#include "logging.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

extern "C"
{
	#include <libcouchbase/couchbase.h>
}

#ifndef COUCHBASE_ADMIN
#define COUCHBASE_ADMIN

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
	void save_object ( Writeable const *obj );
	void create_object ( Writeable const *obj );
	void delete_object ( const char * key );

	//Get the instance, needed for binding callbacks
	lcb_t get_instance ();

	//Blocking call until the transaction stack is empty
	void wait ();
};

#endif
