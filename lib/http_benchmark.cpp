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

//----------------------------HTTP Callbacks----------------------------------//

//A String to store response data
std::string write_data;

//This is the callback that gets called when we recieve the response to the
//Get Curl Request
size_t write_Callback(char * buf, size_t size, size_t nmemb, void* up)
{

  logging->debug("Callback Triggered");

//Put the response into a string
for (int c = 0; c<size*nmemb; c++)
{
	write_data.push_back(buf[c]);
}

return size*nmemb;
}

//----------------------------------------------------------------------------//
//--------------------------------Globals-------------------------------------//
//----------------------------------------------------------------------------//

char *POSTURL;
char *PUTURL;
char *GETURL;
char *DELETEURL;

HttpInterface *http;

//----------------------------------------------------------------------------//
//------------------------------Benchmarks------------------------------------//
//----------------------------------------------------------------------------//

BENCHMARK(HTTP, Get, 10, 100)
{

  //Clear the return string
  write_data.clear();

  //Send the request
  bool success = http->get(GETURL, 5);
  if (!success)
  {
    //We now have the full response
    logging->error("Get Request Failed");
  }
  else
  {
    logging->debug("Retrieved:");
    logging->debug(write_data);
  }

}

BENCHMARK(HTTP, Put, 10, 100)
{

  //Clear the return string
  write_data.clear();

  //Send the request
  bool success = http->put(PUTURL, "123", 5);
  if (!success)
  {
    //We now have the full response
    logging->error("Put Request Failed");
  }
  else
  {
    logging->debug("Retrieved:");
    logging->debug(write_data);
  }

}

BENCHMARK(HTTP, Post, 10, 100)
{

  //Clear the return string
  write_data.clear();

  //Send the request
  bool success = http->post(POSTURL, "CLYMAN", 5);
  if (!success)
  {
    //We now have the full response
    logging->error("Get Request Failed");
  }
  else
  {
    logging->debug("Retrieved:");
    logging->debug(write_data);
  }

}

BENCHMARK(HTTP, Delete, 10, 100)
{

  //Clear the return string
  write_data.clear();

  //Send the request
  bool success = http->del(DELETEURL, 5);
  if (!success)
  {
    //We now have the full response
    logging->error("Get Request Failed");
  }
  else
  {
    logging->debug("Retrieved:");
    logging->debug(write_data);
  }

}

//----------------------------------------------------------------------------//
//------------------------------Main Method-----------------------------------//
//----------------------------------------------------------------------------//

int main()
{

  //Variables to store URL's
  std::string post = "http://httpbin.org/post";
  std::string put = "http://httpbin.org/put";
  std::string get = "http://httpbin.org/get";
  std::string del = "http://httpbin.org/delete";

  POSTURL = new char[post.length() + 1];
  PUTURL = new char[put.length() + 1];
  GETURL = new char[get.length() + 1];
  DELETEURL = new char[del.length() + 1];

  strcpy(POSTURL, post.c_str());
  strcpy(PUTURL, put.c_str());
  strcpy(GETURL, get.c_str());
  strcpy(DELETEURL, del.c_str());

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

//We set up the structure to store the return data
write_data.clear();

http->bind_get_callback(write_Callback);

//------------------------------Run Tests-------------------------------------//
//----------------------------------------------------------------------------//

hayai::ConsoleOutputter consoleOutputter;

hayai::Benchmarker::AddOutputter(consoleOutputter);
hayai::Benchmarker::RunAllTests();

//-------------------------Post-Test Teardown---------------------------------//
//----------------------------------------------------------------------------//

delete http;
delete http_logging;
delete logging;

return 0;

}
