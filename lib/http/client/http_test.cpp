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

#include "include/http_admin.h"
#include "include/factory_http_client.h"
#include "include/factory/http_interface.h"
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

  HttpClientFactory http_factory;
  HttpInterface *ha = http_factory.get_http_interface();

  //-------------------------------GET--------------------------------------//

  //Send the request
  std::string ret_val = ha->get(get, 5);
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

  std::string ret_val2 = ha->get(get, 5);
  if ( ret_val2.empty() )
  {
    //We now have the full response
    assert(false);
  }
  else
  {
    std::cout << ("Retrieved:") << std::endl;
    std::cout << (ret_val2) << std::endl;
  }

  //-------------------------------PUT--------------------------------------//

  bool success = ha->put(put, "123", 5);
  if (!success)
  {
    //We now have the full response
    assert(false);
  }

  //-------------------------------POST-------------------------------------//

  success = ha->post(post, "CLYMAN", 5);
  if (!success)
  {
    //We now have the full response
    assert(false);
  }

  //------------------------------DELETE------------------------------------//

  success = ha->del(del, 5);
  if (!success)
  {
    //We now have the full response
    assert(false);
  }

  ha->shutdown();

  delete ha;

  return 0;
}
