#include "include/http_admin.h"

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

  //----------------------------MAIN TEST-----------------------------------//

  HttpAdmin ha;

  //-------------------------------GET--------------------------------------//

  //Send the request
  std::string ret_val = ha.get(get, 5);
  if ( ret_val.empty() )
  {
    //We now have the full response
    assert(false);
  }
  else
  {
    std::cout << ("Retrieved:") << std::endl;
    std::cout << (ret_val) << std::endl;
  }

  //-------------------------------PUT--------------------------------------//

  bool success = ha.put(put, "123", 5);
  if (!success)
  {
    //We now have the full response
    assert(false);
  }

  //-------------------------------POST-------------------------------------//

  success = ha.post(post, "CLYMAN", 5);
  if (!success)
  {
    //We now have the full response
    assert(false);
  }

  //------------------------------DELETE------------------------------------//

  success = ha.del(del, 5);
  if (!success)
  {
    //We now have the full response
    assert(false);
  }

  std::cout << ("Tests completed") << std::endl;

  ha.shutdown();

  return 0;
}
