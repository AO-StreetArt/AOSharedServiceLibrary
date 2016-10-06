#include "include/http_server.h"
#include "include/factory/callbacks.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <unistd.h>
#include <exception>
#include <signal.h>

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

std::string process_test_request(struct Request *req)
{
	std::cout << "Test Request" << std::endl;
	std::cout << req->req_addr << std::endl;
	return process_request(req);
}

HttpServer *http;

//Shutdown the application
void shutdown()
{
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
http = new HttpServer("127.0.0.1", 12345);
http->bind_callback("/", process_request);
http->bind_callback("/test", process_test_request);

//Listen for Requests
http->recv();

return 0;
}
