# HTTP Server

The HTTP Server provides the architecture to expose a RESTful API for communication with your microservice.
This is the most common framework used for services in other languages.

The HTTP Server binds callbacks to URL's and allows for distinguishing between types of requests.

    #include "aossl/http/server/factory_http_server.h"
    #include "aossl/http/server/http_server_interface.h"

    //Set up the HTTP Server
    HttpServerFactory http_server_factory;
    HttpServerInterface *http = http_server_factory.get_http_server_interface("0.0.0.0", 12345);

    //Bind some callbacks
    http->bind_callback("/", process_request);
    http->bind_callback("/test", process_test_request);

    //Bind a default callback, which is called when a
    //bound callback is not found for the request URL
    //Useful for API's that rely on dynamic URL construction for querying
    http->bind_default_callback(process_default_request);

Here, we bind the specified URL and port to a particular callback function, process_request().
Note that a string is returned from the method with the value being sent back to the client

    std::string process_request(struct Request *req)
    {
      std::string resp = "";
      if (req->req_err->err_code == NOERROR)
      {
        if (req->req_type == HTTP_GET)
        {
          resp = "Get Request";
          std::cout << resp << std::endl;
        }
        else if (req->req_type == HTTP_PUT)
        {
          resp = "Put Request";
          std::cout << resp << std::endl;
        }
        else if (req->req_type == HTTP_POST)
        {
          resp = "Post Request";
          std::cout << resp << std::endl;
          std::cout << req->req_data << std::endl;
        }
        else if (req->req_type == HTTP_DELETE)
        {
          resp = "Delete Request";
          std::cout << resp << std::endl;
        }
        else
        {
          resp = "Unknown Request Type";
          std::cout << resp << req->req_type << std::endl;
        }
      }
      return resp;
    }

And we distinguish between the types via the req_type pointer, as above.

We can also bind a default callback, which get's called whenever a bound URI is not found.
This can be particular useful for allowing access to things like key-value stores

You can see further details on the callbacks for the server [here] (https://github.com/AO-StreetArt/AOSharedServiceLibrary/tree/master/docs/use/drivers/http_server/callbacks)
