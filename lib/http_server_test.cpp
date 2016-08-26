#include "include/http_server.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <unistd.h>
#include <exception>
#include <signal.h>
#include <evhttp.h>

void process_get_request(struct evhttp_request *req, void *arg)
{
	struct evbuffer *buf = evbuffer_new();
	if (buf == NULL) return;
	evbuffer_add_printf(buf, "Get Request", evhttp_request_uri(req));
	evhttp_send_reply(req, HTTP_OK, "OK", buf);
}

void process_post_request(struct evhttp_request *req, void *arg)
{
	struct evbuffer *buf = evbuffer_new();
	if (!buf == NULL)
	{
	evbuffer_add_printf(buf, "Post Request", evhttp_request_uri(req));
	evhttp_send_reply(req, HTTP_OK, "OK", buf);
	}
}

//Process a request
void process_request(struct evhttp_request *req, void *arg){
	std::cout << req->type << std::endl;
	if (req->type == 1)
	{
		process_get_request(req, arg);
	}
	else if (req->type == 2)
	{
		process_post_request(req, arg);
	}
	else if (req->type == 8)
	{
	std::cout << "Put Detected" << std::endl;
	}
	else if (req->type == 16)
	{
	std::cout << "Delete Detected" << std::endl;
	}

}

//Shutdown the application
void shutdown()
{
  //Delete objects off the heap
  delete http;
}

//Catch a Signal (for example, keyboard interrupt)
void my_signal_handler(int s){
   std::cout << ("Caught signal") << std::endl;
   std::string signal_type = std::to_string(s);
   std::cout << (signal_type) << std::endl;
   shutdown();
   exit(1);
}

int main()
{

//Set up a handler for any signal events so that we always shutdown gracefully
struct sigaction sigIntHandler;
sigIntHandler.sa_handler = my_signal_handler;
sigemptyset(&sigIntHandler.sa_mask);
sigIntHandler.sa_flags = 0;
sigaction(SIGINT, &sigIntHandler, NULL);

//Set up the HTTP Server
HttpServer http;
http.bind_callback("0.0.0.0", 12345, process_request);

//Listen for Requests
http.recv();

return 0;
}
