#include "include/http_admin.h"
#include <string>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <assert.h>

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

//-----------------------------MAIN METHOD------------------------------------//

int main()
{
  if (argc > 0)
  {

    //----------------------------TEST SETUP----------------------------------//

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

    //Logging

    std::string initFileName = "test/log4cpp_test.properties";
    try {
    	log4cpp::PropertyConfigurator::configure(initFileName);
    }
    catch ( log4cpp::ConfigureFailure &e ) {
    	printf("[log4cpp::ConfigureFailure] caught while reading Logging Configuration File");
    	printf(e.what());
    	exit(1);
    }

    log4cpp::Category& root = log4cpp::Category::getRoot();

    log4cpp::Category& sub1 = log4cpp::Category::getInstance(std::string("sub1"));

    log4cpp::Category& log = log4cpp::Category::getInstance(std::string("sub1.log"));

    logging = &log;

    //----------------------------MAIN TEST-----------------------------------//

    HttpAdmin ha;

    logging->debug("Get URL");
    logging->debug(GETURL);
    logging->debug("Put URL");
    logging->debug(PUTURL);
    logging->debug("Post URL");
    logging->debug(POSTURL);
    logging->debug("Delete URL");
    logging->debug(DELETEURL);

    //-------------------------------GET--------------------------------------//

    //We set up the structure to store the return data
    writedata.clear();

    curl_easy_setopt(ha.get_instance(), CURLOPT_WRITEFUNCTION, &writeCallback);

    //Send the request
    bool success = ha.get(GETURL, 5);
    if (!success)
    {
      //We now have the full response
      assert(false);
    }
    else
    {
      logging->debug("Retrieved:");
      logging->debug(writedata);
    }

    //-------------------------------PUT--------------------------------------//

    success = ha.put(PUTURL, "123", 5);
    if (!success)
    {
      //We now have the full response
      assert(false);
    }

    //-------------------------------POST-------------------------------------//

    success = ha.post(POSTURL, "CLYMAN", 5);
    if (!success)
    {
      //We now have the full response
      assert(false);
    }

    //------------------------------DELETE------------------------------------//

    success = ha.del(DELETEURL, 5);
    if (!success)
    {
      //We now have the full response
      assert(false);
    }

    logging->debug("Tests completed");

    ha.shutdown();

  }

  return 0;
}
