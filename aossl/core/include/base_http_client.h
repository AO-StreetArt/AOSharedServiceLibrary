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

#include <string>

#include "aossl/core/include/buffers.h"

#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/SSLException.h"

#ifndef AOSSL_CORE_INCLUDE_BASE_HTTP_CLIENT_H_
#define AOSSL_CORE_INCLUDE_BASE_HTTP_CLIENT_H_

namespace AOSSL {

//! Base HTTP Client

//! A Base class which provides convenience methods
//! for accessing HTTP API's.
class BaseHttpClient {
  int timeout;
  bool secured = false;
  bool use_acl = false;
  std::string cert_location;
  std::string http_addr;
  std::string acl_token;
  std::string acl_token_name;

  inline void init(std::string& vaddr, int tout) {
    timeout=tout;
    http_addr.assign(vaddr);
  }

  inline void get_http_response(Poco::Net::HTTPClientSession& session, \
      AOSSL::StringBuffer& ret_buffer) {
    Poco::Net::HTTPResponse res;
    try {
      std::istream& rs = session.receiveResponse(res);
      if (res.getStatus() > Poco::Net::HTTPResponse::HTTP_PARTIAL_CONTENT) {
        ret_buffer.success = false;
        ret_buffer.err_msg.assign("Error sending message to Vault: " + res.getReason());
      } else if (!(rs.good())) {
        ret_buffer.success = false;
        ret_buffer.err_msg.assign("Unknown error retrieving response from Vault");
      } else {
        std::istreambuf_iterator<char> eos;
        std::string resp(std::istreambuf_iterator<char>(rs), eos);
        ret_buffer.success = true;
        ret_buffer.val.assign(resp);
      }
    } catch (std::exception& e) {
      session.reset();
      ret_buffer.success = false;
      ret_buffer.err_msg.assign(e.what());
    }
  }

  inline void send_http_request(Poco::Net::HTTPClientSession& session, \
      Poco::Net::HTTPRequest& req, AOSSL::StringBuffer& ret_buffer) {
    try {
      std::ostream& request_body_inp = session.sendRequest(req);
      if (request_body_inp.good()) {
        get_http_response(session, ret_buffer);
      } else {
        // Reset the HTTP Session after a failure
        session.reset();
        ret_buffer.success = false;
        ret_buffer.err_msg.assign("Unkown Failure sending HTTP Request");
      }
    } catch (std::exception& e) {
      session.reset();
      ret_buffer.success = false;
      ret_buffer.err_msg.assign(e.what());
    }
  }

  inline void send_http_request(Poco::Net::HTTPClientSession& session, \
      Poco::Net::HTTPRequest& req, std::string& body, AOSSL::StringBuffer& ret_buffer) {
    req.setContentLength(body.length());
    try {
      std::ostream& request_body_inp = session.sendRequest(req);
      if (request_body_inp.good()) {
         request_body_inp << body;
         get_http_response(session, ret_buffer);
      } else {
        // Reset the HTTP Session after a failure
        session.reset();
        ret_buffer.success = false;
        ret_buffer.err_msg.assign("Unkown Failure sending HTTP Request");
      }
    } catch (std::exception& e) {
      session.reset();
      ret_buffer.success = false;
      ret_buffer.err_msg.assign(e.what());
    }
  }

  inline void create_and_send_request(std::string& req_type, bool use_body, \
      std::string& query_url, std::string& body, AOSSL::StringBuffer& ret_buffer) {
    ret_buffer.success = false;
    try {
      // Build the base HTTP Request
      const Poco::URI uri( http_addr );
      Poco::Net::HTTPRequest req(req_type, query_url );
      // Add the acl token if it's been added
      if (use_acl) {
        req.add(acl_token_name, acl_token);
      }
      // Send the request over a secured/unsecured socket based on config
      if (secured) {
        const Poco::Net::Context::Ptr context( new Poco::Net::Context( \
            Poco::Net::Context::CLIENT_USE, "", cert_location, "", \
            Poco::Net::Context::VerificationMode::VERIFY_NONE ) );
        Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort(), context );
        if (use_body) {
          send_http_request(session, req, body, ret_buffer);
        } else {
          send_http_request(session, req, ret_buffer);
        }
      } else {
        Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());
        if (use_body) {
          send_http_request(session, req, body, ret_buffer);
        } else {
          send_http_request(session, req, ret_buffer);
        }
      }
    } catch( const Poco::Net::SSLException& e ) {
      ret_buffer.success = false;
      ret_buffer.err_msg.assign(e.message());
    } catch( const std::exception& e ) {
      ret_buffer.success = false;
      ret_buffer.err_msg.assign(e.what());
    }
  }

 public:
   //! Construct a secured HTTP Client
  BaseHttpClient(std::string& vaddr, int tout, std::string& cert) \
      {init(vaddr, tout);secured=true;cert_location.assign(cert);}
  //! Construct an unsecured HTTP Client
  BaseHttpClient(std::string& vaddr, int tout) \
      {init(vaddr, tout);}
  ~BaseHttpClient() {}

  // View configuration elements
  std::string get_address() {return http_addr;}
  std::string get_ssl_cert() {return cert_location;}
  std::string get_acl_token() {return acl_token;}

  //! Execute a Put query
  inline void put_by_reference(std::string& query_url, std::string& body, \
      AOSSL::StringBuffer& ret_buffer) {
    std::string req_type = Poco::Net::HTTPRequest::HTTP_PUT;
    create_and_send_request(req_type, true, query_url, body, ret_buffer);
  }

  //! Execute a Post query
  inline void post_by_reference(std::string& query_url, std::string& body, \
      AOSSL::StringBuffer& ret_buffer) {
    std::string req_type = Poco::Net::HTTPRequest::HTTP_POST;
    create_and_send_request(req_type, true, query_url, body, ret_buffer);
  }

  //! Execute a Get query
  inline void get_by_reference(std::string& query_url, AOSSL::StringBuffer& ret_buffer) {
    std::string body = "";
    std::string req_type = Poco::Net::HTTPRequest::HTTP_GET;
    create_and_send_request(req_type, false, query_url, body, ret_buffer);
  }

  //! Execute a Delete query
  inline void delete_by_reference(std::string& query_url, AOSSL::StringBuffer& ret_buffer) {
    std::string body = "";
    std::string req_type = Poco::Net::HTTPRequest::HTTP_DELETE;
    create_and_send_request(req_type, false, query_url, body, ret_buffer);
  }
  //! Add an ACL Token
  inline void set_acl_token(std::string token_name, std::string new_token) {
    acl_token.assign(new_token);
    acl_token_name.assign(token_name);
    use_acl = true;
  }
};

}  // namespace AOSSL

#endif  // AOSSL_CORE_INCLUDE_BASE_HTTP_CLIENT_H_
