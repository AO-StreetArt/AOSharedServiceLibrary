#include "couchbase_admin.h"

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
        private_instance=instance;
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

void CouchbaseAdmin::save_object ( Writeable const *obj )
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

void CouchbaseAdmin::create_object ( Writeable const *obj )
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
