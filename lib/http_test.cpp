#include "include/http_admin.h"
#include "include/factory/logging_interface.h"
#include "include/logging.h"

#include <string>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <assert.h>

//-----------------------------MAIN METHOD------------------------------------//

int main()
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
    logging = new Logger(initFileName);

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

    //Send the request
    std::string get_response = ha.get(GETURL, 5);
    if (!get_response.empty())
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

    std::string success = ha.put(PUTURL, "123", 5);

    //-------------------------------POST-------------------------------------//

    success = ha.post(POSTURL, "CLYMAN", 5);

    //------------------------------DELETE------------------------------------//

    success = ha.del(DELETEURL, 5);

    logging->debug("Tests completed");

    ha.shutdown();

  delete logging;

  return 0;
}
