# HTTP Administrator

The HTTP Admin allows quick and easy HTTP Requests by exposing:

* bool put(std::string url, std::string data, int timeout)
* std::string get(std::string url, int timeout)
* bool post(std::string url, std::string data, int timeout)
* bool del(std::string url, int timeout)

    #include "include/factory_http_client.h"
    #include "include/factory/http_interface.h"

    HttpClientFactory http_client_factory;

    //Declare the admin
    HttpInterface *ha = http_client_factory.get_http_interface();

    //-------------------------------GET--------------------------------------//

    //Send the request
    std::string returned_string = ha->get(GETURL, 5);

    //-------------------------------PUT--------------------------------------//
    success = ha->put(PUTURL, "123", 5);

    //-------------------------------POST-------------------------------------//
    success = ha->post(POSTURL, "CLYMAN", 5);

    //------------------------------DELETE------------------------------------//
    success = ha->del(DELETEURL, 5);
