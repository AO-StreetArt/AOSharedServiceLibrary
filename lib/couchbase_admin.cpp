#include "include/couchbase_admin.h"

CouchbaseSession *current_couchbase_session = NULL;

void CouchbaseAdmin::initialize (const char * conn)
{
  //Start the Couchbase Session, if necessary
  if (!current_couchbase_session) {
    current_couchbase_session = new CouchbaseSession;
  }

	//Initializing
  lcb_create_st cropts;
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

  lcb_install_callback3(private_instance, LCB_CALLBACK_GET, get_callback);
  lcb_install_callback3(private_instance, LCB_CALLBACK_STORE, storage_callback);
  lcb_install_callback3(private_instance, LCB_CALLBACK_REMOVE, del_callback);
}

void CouchbaseAdmin::save (std::string key, std::string val, int op_code) {
  lcb_error_t err;
	lcb_CMDSTORE scmd = { 0 };
  LCB_CMD_SET_KEY(&scmd, key.c_str(), key.length());
  LCB_CMD_SET_VALUE(&scmd, val.c_str(), val.length())
  if (op_code == COUCHBASE_CREATE) {
	   scmd.operation = LCB_SET;
  }
  else if (op_code == COUCHBASE_UPDATE) {
    scmd.operation = LCB_REPLACE;
  }
  else {
    throw CouchbaseOperationException( "Couchbase Operation Code not found" );
  }
	err = lcb_store3(private_instance, NULL, &scmd);
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

  lcb_CMDGET gcmd = { 0 };
  LCB_CMD_SET_KEY(&gcmd, key, strlen(key));

	//Schedule a Get operation
	err = lcb_get3(private_instance, NULL, &gcmd);
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

  lcb_CMDREMOVE rcmd = { 0 };
  LCB_CMD_SET_KEY(&rcmd, key, strlen(key));

	err = lcb_remove3(private_instance, NULL, &rcmd);
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
