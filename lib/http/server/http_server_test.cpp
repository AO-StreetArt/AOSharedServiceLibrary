/*
MIT License Block

Copyright (c) 2016 Alex Barry

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "include/factory_http_server.h"
#include "include/http_server_interface.h"
#include "include/callbacks.h"
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

HttpServerInterface *http;

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

	HttpServerFactory server_factory;

	//Set up the HTTP Server
	http = server_factory.get_http_server_interface("127.0.0.1", 12345);
	http->bind_callback("/", process_request);
	http->bind_callback("/test", process_test_request);

	//Listen for Requests
	http->recv();

	return 0;
}
