#include "include/couchbase_admin.h"

CallbackInterface storage;
CallbackInterface retrieval;
CallbackInterface deletion;

//Transfer the callback information into a Request and call the registered callback
static void storage_callback(lcb_t instance, const void *cookie, lcb_storage_t op,
   lcb_error_t err, const lcb_store_resp_t *resp)
{
	//Build the request
	Request *r = new Request();
  RequestError *rerr = r->req_err;

	//Get the Key
	std::string key ((char*)resp->v.v0.key), (int)resp->v.v0.nkey);
	r->req_addr = key;

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

}

static void get_callback(lcb_t instance, const void *cookie, lcb_error_t err,
   const lcb_get_resp_t *resp)
{
	//Build the request
	Request *r = new Request();
  RequestError *rerr = r->req_err;

	//Get the Key
	std::string key ((char*)resp->v.v0.key), (int)resp->v.v0.nkey);
	r->req_addr = key;

	//Get the retrieved value
	std::string val ((char*)resp->v.v0.bytes, (int)resp->v.v0.nbytes);

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
}

static void del_callback(lcb_t instance, const void *cookie, lcb_error_t err, const lcb_remove_resp_t *resp)
{
	//Build the request
	Request *r = new Request();
  RequestError *rerr = r->req_err;

	//Get the Key
	std::string key ((char*)resp->v.v0.key), (int)resp->v.v0.nkey);
	r->req_addr = key;

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
}

void CouchbaseAdmin::initialize (const char * conn)
{
	//Initializing
        logging->info("CB_Admin:DB: Couchbase Admin Initializing");
        struct lcb_create_st cropts;
        cropts.version = 3;
        cropts.v.v3.connstr = conn;

	//Add a password if authentication is active
	if (authentication_active == true) {
		cropts.v.v3.passwd = password;
	}

        lcb_error_t err;
        lcb_t instance;
        private_instance = instance;
        err = lcb_create(&private_instance, &cropts);
        if (err != LCB_SUCCESS) {
                logging->error("CB_Admin:DB: Couldn't create instance!");
        }

        //Connecting
        lcb_connect(private_instance);
        lcb_wait(private_instance);
        if ( (err = lcb_get_bootstrap_status(private_instance)) != LCB_SUCCESS ) {
                logging->error("CB_Admin:DB: Couldn't bootstrap!");
        }

    lcb_set_remove_callback(private_instance, del_callback);
    lcb_set_store_callback(private_instance, storage_callback);
    lcb_set_get_callback(private_instance, get_callback);
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
	logging->info("CB_Admin:DB: Object being loaded with key:");
	logging->info(key);
	//Initialize the variables
	lcb_error_t err;
	lcb_get_cmd_t gcmd;
	const lcb_get_cmd_t *gcmdlist = &gcmd;
	gcmd.v.v0.key = key;
	gcmd.v.v0.nkey = strlen(key);

	//Schedule a Get operation
	err = lcb_get(private_instance, NULL, 1, &gcmdlist);
	if (err != LCB_SUCCESS) {
		logging->error("CB_Admin:DB: Couldn't schedule get operation!");
	}
}

void CouchbaseAdmin::save_object ( Writeable *obj )
{
	logging->info("CB_Admin:DB: Object being saved");
	lcb_store_cmd_t scmd;
	lcb_error_t err;
        const lcb_store_cmd_t *scmdlist = &scmd;
        std::string key = obj->get_key();
        scmd.v.v0.key = key.c_str();
        scmd.v.v0.nkey = key.length();
		std::string obj_json_str = obj->to_json();
        const char * object_string = obj_json_str.c_str();
        scmd.v.v0.bytes = object_string;
        scmd.v.v0.nbytes = strlen(object_string);
        scmd.v.v0.operation = LCB_REPLACE;
        err = lcb_store(private_instance, NULL, 1, &scmdlist);
        if (err != LCB_SUCCESS) {
                logging->error("CB_Admin:Couldn't schedule storage operation!");
        }
}

void CouchbaseAdmin::create_object ( Writeable *obj )
{
	logging->info("CB_Admin:Create Object Called");
	lcb_error_t err;
	lcb_store_cmd_t scmd;
	const lcb_store_cmd_t *scmdlist = &scmd;
	std::string key = obj->get_key();
	scmd.v.v0.key = key.c_str();
	scmd.v.v0.nkey = key.length();
	std::string obj_json_str = obj->to_json();
	const char * object_string = obj_json_str.c_str();
	scmd.v.v0.bytes = object_string;
	scmd.v.v0.nbytes = strlen(object_string);
	scmd.v.v0.operation = LCB_SET;
	err = lcb_store(private_instance, NULL, 1, &scmdlist);
	if (err != LCB_SUCCESS) {
		logging->error("CB_Admin:Couldn't schedule storage operation!");
	}
}

void CouchbaseAdmin::delete_object ( const char * key ) {
	logging->info("CB_Admin:Delete Object Called with key: ");
	logging->info(key);
	lcb_error_t err;
	lcb_remove_cmd_t cmd;
	const lcb_remove_cmd_t *cmdlist = &cmd;
	cmd.v.v0.key = key;
	cmd.v.v0.nkey = strlen(key);
	err = lcb_remove(private_instance, NULL, 1, &cmdlist);
	if (err != LCB_SUCCESS) {
		logging->error("CB_Admin:Couldn't schedule remove operation: ");
		logging->error( lcb_strerror(private_instance, err));
	}
}

//Bind the Get Callback for the couchbase calls
void CouchbaseAdmin::bind_get_callback(CallbackInterface gc)
{
  retrieval = gc;
}

//Bind the Storage Callback for the couchbase calls
void CouchbaseAdmin::bind_storage_callback(CallbackInterface sc)
{
  storage = sc;
}

//Bind the Delete Callback for the couchbase calls
void CouchbaseAdmin::bind_delete_callback(CallbackInterface dc)
{
  deletion = dc;
}

lcb_t CouchbaseAdmin::get_instance ()
{
	return private_instance;
}

void CouchbaseAdmin::wait ()
{
	logging->info("CB_Admin:DB: Clear Function Stack Called");
	lcb_wait(private_instance);
	logging->info("CB_Admin:Done waiting");
}
