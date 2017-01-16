# Callbacks

Request-based callbacks are critical components of AOSSL, and can be seen used with several AOSSL components, in particular the HTTP Server and Couchbase Administrator.  All callbacks match the following method signature:
    std::string process_request(struct Request *req)

The Request object has a few properties which are relevant:

* req_data - Used to store original request data
* req_addr - Used to store the request type
* req_err - A pointer used to store any error messages from the request
* resp_data - Used to store response data from the request

The Request Error, if present, will come in the form of:

    //! A struct that gets passed to callbacks to transmit errors
    struct RequestError
    {
      //! A numerical error code
      int err_code;

      //! An Error message
      std::string err_message;
      RequestError() {err_code = NOERROR; err_message = "";}
    };
