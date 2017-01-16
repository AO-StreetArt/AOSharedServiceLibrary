# Couchbase Interface

Couchbase is a document-based database that allows for persistent, clusterable storage of data objects
for services.

The Couchbase Administrator works with objects that extend the interface 'writeable.h', found in the include directory.

`class TestData: public Writeable`

We start by defining a class which extends the Writeable interface.  This requires only that we have three
methods implemented:

* get_key
* set_key
* to_json

Now, we can start building objects in our DB:

    #include "include/factory_couchbase.h"
    #include "include/factory/couchbase_interface.h"

    CouchbaseComponentFactory couchbase_factory;

    //Create an object
    std::string name = "TestObject";
    TestData data ();
    data.set_key(name);
    const char* obj_key = data.get_key().c_str();

    //Build the Couchbase Admin (which will automatically connect to the DB),
    //by asking the factory to create the new instance
    CouchbaseInterface *cb = couchbase_factory.get_couchbase_interface("couchbase://localhost/default");

    //Bind callbacks
    cb->bind_get_callback(my_retrieval_callback);
    cb->bind_storage_callback(my_storage_callback);
    cb->bind_delete_callback(my_delete_callback);

Please note that couchbase binds to callbacks for all operations.  Here we see a simple example of the request callbacks:

    std::string my_storage_callback (Request *r)
    {
      if (r->req_err->err_code == NOERROR)
      {
        std::cout << "stored: " << r->req_addr << std::endl;
      }
      else
      {
        std::cout << "Failed to Store: " << r->req_addr << std::endl;
        std::cout << r->req_err->err_message << std::endl;
      }
      return r->req_addr;
    }

    std::string my_retrieval_callback (Request *r)
    {
      if (r->req_err->err_code == NOERROR)
      {
        std::cout << "retrieved: " << r->req_addr << std::endl;
        std::cout << "value: " << r->req_data << std::endl;
      }
      else
      {
        std::cout << "Failed to Store: " << r->req_addr << std::endl;
        std::cout << r->req_err->err_message << std::endl;
      }
      return r->req_addr;
    }

    std::string my_delete_callback (Request *r)
    {
      if (r->req_err->err_code == NOERROR)
      {
        std::cout << "removed: " << r->req_addr << std::endl;
      }
      else
      {
        std::cout << "Failed to Delete: " << r->req_addr << std::endl;
        std::cout << r->req_err->err_message << std::endl;
      }
      return r->req_addr;
    }


Once our callbacks are bound, we can start operating with Couchbase:

    //Write the object to the DB
    cb->create_object ( obj_ptr );
    cb->wait();
    //Get the object from the DB
    cb->load_object ( obj_key );
    cb->wait();
    //Update the object in the DB
    data.set_i ( 10 );
    cb->save_object ( obj_ptr );
    cb->wait();
    //Get the object from the DB to ensure it updates correctly
    cb->load_object ( obj_key );
    cb->wait();
    //Delete the object
    cb->delete_object ( obj_key );
    cb->wait();
