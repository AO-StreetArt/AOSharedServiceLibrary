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

#include <hayai/hayai.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <uuid/uuid.h>

#include "include/factory/logging_interface.h"
#include "include/factory/http_interface.h"
#include "include/factory_logging.h"
#include "include/factory_http_client.h"

//----------------------------------------------------------------------------//
//--------------------------------Globals-------------------------------------//
//----------------------------------------------------------------------------//

std::string post = "http://httpbin.org/post";
std::string put = "http://httpbin.org/put";
std::string get = "http://httpbin.org/get";
std::string del = "http://httpbin.org/delete";

HttpInterface *http;

//----------------------------------------------------------------------------//
//------------------------------Benchmarks------------------------------------//
//----------------------------------------------------------------------------//

BENCHMARK(HTTP, Get, 10, 100)
{


  //Send the request
  std::string ret_val = http->get(get, 5);
  if (ret_val.empty())
  {
    logging->error("Get Request Failed");
  }

}

BENCHMARK(HTTP, Put, 10, 100)
{


  //Send the request
  bool success = http->put(put, "123", 5);
  if (!success)
  {
    //We now have the full response
    logging->error("Put Request Failed");
  }

}

BENCHMARK(HTTP, Post, 10, 100)
{

  //Send the request
  bool success = http->post(post, "CLYMAN", 5);
  if (!success)
  {
    //We now have the full response
    logging->error("Post Request Failed");
  }

}

BENCHMARK(HTTP, Delete, 10, 100)
{

  //Send the request
  bool success = http->del(del, 5);
  if (!success)
  {
    //We now have the full response
    logging->error("Delete Request Failed");
  }

}

//----------------------------------------------------------------------------//
//------------------------------Main Method-----------------------------------//
//----------------------------------------------------------------------------//

int main()
{

  HttpClientFactory http_client_factory;
  LoggingComponentFactory logging_factory;

//Read the Logging Configuration File
std::string initFileName = "test/log4cpp_test.properties";
logging = logging_factory.get_logging_interface( initFileName );

//Set up internal variables
logging->info("Internal Logging Intialized");

//Set up HTTP Client
http = http_client_factory.get_http_interface();
logging->info("HTTP Outbound Interface Created");

//------------------------------Run Tests-------------------------------------//
//----------------------------------------------------------------------------//

hayai::ConsoleOutputter consoleOutputter;

hayai::Benchmarker::AddOutputter(consoleOutputter);
hayai::Benchmarker::RunAllTests();

//-------------------------Post-Test Teardown---------------------------------//
//----------------------------------------------------------------------------//

delete http;
delete logging;

return 0;

}
