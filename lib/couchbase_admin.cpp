#include "include/couchbase_admin.h"

CouchbaseSession *current_couchbase_session = NULL;

void CouchbaseAdmin::initialize (const char * conn)
{
  //Start the Couchbase Session, if necessary
  if (!current_couchbase_session) {
    current_couchbase_session = new CouchbaseSession;
  }

	//Initializing
  struct lcb_create_st cropts;
  memset(&cropts, 0, sizeof cropts);
  cropts.version = 3;
  cropts.v.v3.connstr = conn;

	//Add a password if authentication is active
	if (authentication_active == true) {
		cropts.v.v3.passwd = password;
	}

  //Couchbase Connection Creation

  //Schedule Bootstrap Creation
  lcb_error_t err = lcb_create(&private_instance, &cropts);
  if (err != LCB_SUCCESS) {
    throw CouchbaseInitException(lcb_strerror(NULL, err));
  }

  //Schedule Connection
  err = lcb_connect(private_instance);
  if (err != LCB_SUCCESS) {
    throw CouchbaseConnectException(lcb_strerror(NULL, err));
  }

  //Yield to IO
  lcb_wait(private_instance);
  err = lcb_get_bootstrap_status(private_instance);
  if (err != LCB_SUCCESS) {
    throw CouchbaseBootstrapException(lcb_strerror(NULL, err));
  }

  lcb_set_remove_callback(private_instance, del_callback);
  lcb_set_store_callback(private_instance, storage_callback);
  lcb_set_get_callback(private_instance, get_callback);
}

void save (std::string key, std::string val, int op_code) {
  lcb_error_t err;
	lcb_store_cmd_t scmd;
	const lcb_store_cmd_t *scmdlist = &scmd;
	scmd.v.v0.key = key.c_str();
	scmd.v.v0.nkey = key.length();
	const char * object_string = val.c_str();
	scmd.v.v0.bytes = object_string;
	scmd.v.v0.nbytes = strlen(object_string);
  if (op_code == COUCHBASE_CREATE) {
	   scmd.v.v0.operation = LCB_SET;
  }
  else if (op_code == COUCHBASE_UPDATE) {
    scmd.v.v0.operation = LCB_REPLACE;
  }
  else {
    throw CouchbaseOperationException( "Couchbase Operation Code not found" );
  }
	err = lcb_store(private_instance, NULL, 1, &scmdlist);
	if (err != LCB_SUCCESS) {
		throw CouchbaseOperationException( lcb_strerror(private_instance, err) );
	}
}

CouchbaseAdmin::CouchbaseAdmin( const char * conn )
{
	authentication_active = false;
	initialize (conn);
}

CouchbaseAdmin::CouchbaseAdmin( const char * conn, const char * pswd )
{
        authentication_active = true;
	password = pswd;
	initialize (conn);
}

CouchbaseAdmin::~CouchbaseAdmin ()
{
	lcb_destroy(private_instance);
}

void CouchbaseAdmin::load_object ( const char * key )
{
	//Initialize the variables
	lcb_error_t err;
	lcb_get_cmd_t gcmd;
	const lcb_get_cmd_t *gcmdlist = &gcmd;
	gcmd.v.v0.key = key;
	gcmd.v.v0.nkey = strlen(key);

	//Schedule a Get operation
	err = lcb_get(private_instance, NULL, 1, &gcmdlist);
	if (err != LCB_SUCCESS) {
		throw CouchbaseOperationException( lcb_strerror(private_instance, err) );
	}
}

void CouchbaseAdmin::load_object ( std::string key ) {
  load_object( key.c_str() );
}

void CouchbaseAdmin::save_object ( Writeable *obj )
{
  save (obj->get_key(), obj->to_json(), COUCHBASE_UPDATE);
}

void CouchbaseAdmin::create_object ( Writeable *obj )
{
  save (obj->get_key(), obj->to_json(), COUCHBASE_CREATE);
}

void CouchbaseAdmin::save_string ( std::string key, std::string val ) {
  save (key, val, COUCHBASE_UPDATE);
}

void CouchbaseAdmin::create_string ( std::string key, std::string val ) {
  save (key, val, COUCHBASE_CREATE);
}

void CouchbaseAdmin::delete_object ( const char * key ) {
	lcb_error_t err;
	lcb_remove_cmd_t cmd;
	const lcb_remove_cmd_t *cmdlist = &cmd;
	cmd.v.v0.key = key;
	cmd.v.v0.nkey = strlen(key);
	err = lcb_remove(private_instance, NULL, 1, &cmdlist);
	if (err != LCB_SUCCESS) {
		throw CouchbaseOperationException( lcb_strerror(private_instance, err) );
	}
}

void CouchbaseAdmin::delete_object ( std::string key ) {
  delete_object( key.c_str() );
}

//Bind the Get Callback for the couchbase calls
void CouchbaseAdmin::bind_get_callback(CallbackInterface gc)
{
  current_couchbase_session->retrieval = gc;
}

//Bind the Storage Callback for the couchbase calls
void CouchbaseAdmin::bind_storage_callback(CallbackInterface sc)
{
  current_couchbase_session->storage = sc;
}

//Bind the Delete Callback for the couchbase calls
void CouchbaseAdmin::bind_delete_callback(CallbackInterface dc)
{
  current_couchbase_session->deletion = dc;
}

lcb_t CouchbaseAdmin::get_instance ()
{
	return private_instance;
}

void CouchbaseAdmin::wait ()
{
	lcb_wait(private_instance);
}
