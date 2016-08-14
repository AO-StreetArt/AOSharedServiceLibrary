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
#include "include/factory.h"

//----------------------------HTTP Callbacks----------------------------------//

//A String to store response data
std::string writedata;

//This is the callback that gets called when we recieve the response to the
//Get Curl Request
size_t writeCallback(char * buf, size_t size, size_t nmemb, void* up)
{

  logging->debug("Callback Triggered");

//Put the response into a string
for (int c = 0; c<size*nmemb; c++)
{
	writedata.push_back(buf[c]);
}

return size*nmemb;
}

//----------------------------------------------------------------------------//
//--------------------------------Globals-------------------------------------//
//----------------------------------------------------------------------------//

//Variables to store URL's
std::string post = "http://httpbin.org/post";
std::string put = "http://httpbin.org/put";
std::string get = "http://httpbin.org/get";
std::string del = "http://httpbin.org/delete";

char *POSTURL = new char[post.length() + 1];
strcpy(POSTURL, post.c_str());

char *PUTURL = new char[put.length() + 1];
strcpy(PUTURL, put.c_str());

char *GETURL = new char[get.length() + 1];
strcpy(GETURL, get.c_str());

char *DELETEURL = new char[del.length() + 1];
strcpy(DELETEURL, del.c_str());

HttpInterface *http;

//----------------------------------------------------------------------------//
//------------------------------Benchmarks------------------------------------//
//----------------------------------------------------------------------------//

BENCHMARK(HTTP, Get, 10, 100)
{

  //Clear the return string
  writedata.clear();

  //Send the request
  bool success = ha.get(GETURL, 5);
  if (!success)
  {
    //We now have the full response
    logging->error("Get Request Failed");
  }
  else
  {
    logging->debug("Retrieved:");
    logging->debug(writedata);
  }

}

BENCHMARK(HTTP, Put, 10, 100)
{

  //Clear the return string
  writedata.clear();

  //Send the request
  bool success = ha.put(PUTURL, "123", 5);
  if (!success)
  {
    //We now have the full response
    logging->error("Put Request Failed");
  }
  else
  {
    logging->debug("Retrieved:");
    logging->debug(writedata);
  }

}

BENCHMARK(HTTP, Post, 10, 100)
{

  //Clear the return string
  writedata.clear();

  //Send the request
  bool success = ha.post(POSTURL, "CLYMAN", 5);
  if (!success)
  {
    //We now have the full response
    logging->error("Get Request Failed");
  }
  else
  {
    logging->debug("Retrieved:");
    logging->debug(writedata);
  }

}

BENCHMARK(HTTP, Delete, 10, 100)
{

  //Clear the return string
  writedata.clear();

  //Send the request
  bool success = ha.del(DELETEURL, 5);
  if (!success)
  {
    //We now have the full response
    logging->error("Get Request Failed");
  }
  else
  {
    logging->debug("Retrieved:");
    logging->debug(writedata);
  }

}

//----------------------------------------------------------------------------//
//------------------------------Main Method-----------------------------------//
//----------------------------------------------------------------------------//

int main()
{

ServiceComponentFactory factory;

//Read the Logging Configuration File
std::string initFileName = "src/test/log4cpp_test.properties";
logging = factory.get_logging_interface( initFileName );
//logging = new Logger(initFileName);

//Set up internal variables
logging->info("Internal Logging Intialized");

//Set up UUID Generator
http = factory.get_http_interface();
//uuid = new uuidAdmin;
logging->info("HTTP Outbound Interface Created");

//We set up the structure to store the return data
writedata.clear();

http.bind_get_callback(writeCallback);

//------------------------------Run Tests-------------------------------------//
//----------------------------------------------------------------------------//

hayai::ConsoleOutputter consoleOutputter;

hayai::Benchmarker::AddOutputter(consoleOutputter);
hayai::Benchmarker::RunAllTests();

//-------------------------Post-Test Teardown---------------------------------//
//----------------------------------------------------------------------------//

delete uuid
delete logging;

return 0;

}
