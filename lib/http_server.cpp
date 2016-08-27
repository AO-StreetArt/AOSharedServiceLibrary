#include "include/http_server.h"

std::unordered_map<std::string, CallbackInterface> callback_map;

//Process a request
void process_request(struct evhttp_request *req, void *arg){

  //Form a Request object from the evhttp request data, and call the appropriate callback
  //Then, we return a string in the callback that's sent in the response, and check the
  //Error object stored within the request pointer passed into the method

  Request *r = new Request();
  RequestError *err = r->req_err;
  const char * uri_cstr = evhttp_request_uri(req);
  std::string uri (uri_cstr);
  r->req_addr = uri;

  //Get the data out of the evhttp request buffer
  try
  {
  	size_t len = evbuffer_get_length(evhttp_request_get_input_buffer(req));
  	struct evbuffer *in_evb = evhttp_request_get_input_buffer(req);
  	char *data = (char*)malloc(len);
  	evbuffer_copyout(in_evb, data, len);

    //Put the data into the universal request struct
    std::string rdata(data, len);
    r->req_data = rdata;
    free(data);
  }
  catch (std::exception& e)
  {
    err->err_code = HTTP_BADREQUEST;
    err->err_message = e.what();
  }

  //Determine the Request type
  if (req->type == 1)
	{
		//Get Request
    r->req_type = HTTP_GET;
	}
	else if (req->type == 2)
	{
		//Post Request
    r->req_type = HTTP_POST;
	}
	else if (req->type == 8)
	{
  	//Put Request
    r->req_type = HTTP_PUT;
	}
	else if (req->type == 16)
	{
	   //Delete Request
     r->req_type = HTTP_DELETE;
	}
  else
  {
    //Unknown Request type
    r->req_type = UNKNOWN;
  }

  std::string resp = "";
  CallbackInterface cb;

  //Call the callback, store the return value in a string that's sent in a response
  try
  {
    auto search = callback_map.find(uri);
    if(search != callback_map.end()) {
      cb = callback_map[uri];
      try
      {
        resp = (*cb)(r);
      }
      catch (std::exception& e)
      {
        err->err_code = HTTP_BADREQUEST;
        err->err_message = e.what();
      }
    }
  }
  catch (std::exception& e)
  {
    err->err_code = HTTP_NOTFOUND;
    err->err_message = e.what();
  }

  struct evbuffer *buf = evbuffer_new();
	if (buf != NULL) {
	  evbuffer_add_printf(buf, resp.c_str(), uri_cstr);

    //Send a success response if no error is detected
    if (r->req_err->err_code == NOERROR)
    {
  	   evhttp_send_reply(req, HTTP_OK, "OK", buf);
    }
    //Send a failure response
    else
    {
      evhttp_send_reply(req, err->err_code, err->err_message.c_str(), buf);
    }
  }

}

//HTTP Server Methods

HttpServer::HttpServer(std::string base_addr, int base_port)
{
  base = NULL;
  base = event_init();
  internal_url = base_addr;
  internal_port = base_port;

  //Bind the address to the internal callback method
  struct evhttp *httpd;
  httpd = evhttp_new(base);
  if (httpd != NULL) {
    if (evhttp_bind_socket(httpd, internal_url.c_str(), internal_port) == 0) {
      evhttp_set_gencb(httpd, process_request, NULL);
    }
  }
}

HttpServer::~HttpServer()
{
  // if (base != NULL) delete base;
}

bool HttpServer::bind_callback(std::string uri, CallbackInterface func)
{
  //Add the address & Callback to the function pointer map
  callback_map.emplace(uri, func);
  return true;
}

void HttpServer::recv()
{
  event_base_dispatch(base);
}
