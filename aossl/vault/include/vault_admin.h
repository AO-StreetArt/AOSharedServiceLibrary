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
#include "aossl/vault/include/vault_interface.h"

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

#include "Poco/StreamCopier.h"
#include "Poco/URI.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/SSLException.h"

#ifndef AOSSL_VAULT_INCLUDE_VAULT_ADMIN_H_
#define AOSSL_VAULT_INCLUDE_VAULT_ADMIN_H_

namespace AOSSL {

const int APPROLE_AUTH_TYPE = 0;
const int BASIC_AUTH_TYPE = 1;

//! Vault Admin

//! A Key-Value store for accessing secured values in Vault
class VaultAdmin : public VaultInterface {
  int timeout;
  bool secured = false;
  std::string cert_location;
  std::string vault_addr;
  std::string secrets_url_path;
  std::string vault_token;
  std::string username;
  std::string password;
  bool is_authenticated = false;
  int authentication_type;
  inline void init(std::string& vaddr, std::string& secrets_path, int tout, \
      std::string& un, std::string& pw, int auth_type) {
    timeout=tout;
    vault_addr.assign(vaddr);
    secrets_url_path.assign(secrets_path);
    username.assign(un);
    password.assign(pw);
    authentication_type = auth_type;
  }
  inline void get_http_response(Poco::Net::HTTPClientSession& session, \
      AOSSL::StringBuffer& ret_buffer) {
    Poco::Net::HTTPResponse res;
    std::istream& rs = session.receiveResponse(res);
    if (res.getStatus() > Poco::Net::HTTPResponse::HTTP_PARTIAL_CONTENT) {
      ret_buffer.success = false;
      ret_buffer.err_msg.assign("Error sending message to Vault: " + res.getReason());
    } else {
      std::istreambuf_iterator<char> eos;
      std::string resp(std::istreambuf_iterator<char>(rs), eos);
      ret_buffer.success = true;
      ret_buffer.val.assign(resp);
    }
  }
  inline void send_http_request(Poco::Net::HTTPClientSession& session, \
      Poco::Net::HTTPRequest& req, AOSSL::StringBuffer& ret_buffer) {
    session.sendRequest(req);
    get_http_response(session, ret_buffer);
  }
  inline void send_http_request(Poco::Net::HTTPClientSession& session, \
      Poco::Net::HTTPRequest& req, std::string& body, AOSSL::StringBuffer& ret_buffer) {
    req.setContentLength(body.length());
    session.sendRequest(req) << body;
    get_http_response(session, ret_buffer);
  }
  inline void create_and_send_request(std::string& req_type, bool use_body, \
      std::string& query_url, std::string& body, AOSSL::StringBuffer& ret_buffer) {
    ret_buffer.success = false;
    try {
      // Build the base HTTP Request
      const Poco::URI uri( vault_addr );
      Poco::Net::HTTPRequest req(req_type, query_url );
      // Add the vault token if we've authenticated
      if (is_authenticated) {
        req.add("X-Vault-Token", vault_token);
      }
      // Send the request over a secured/unsecured socket based on config
      if (secured) {
        const Poco::Net::Context::Ptr context( new Poco::Net::Context( Poco::Net::Context::CLIENT_USE, "", "", cert_location ) );
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
  // Execute a Put query
  inline void put_by_reference(std::string& query_url, std::string& body, \
      AOSSL::StringBuffer& ret_buffer) {
    std::string req_type = Poco::Net::HTTPRequest::HTTP_PUT;
    create_and_send_request(req_type, true, query_url, body, ret_buffer);
  }
  // Execute a Post query
  inline void post_by_reference(std::string& query_url, std::string& body, \
      AOSSL::StringBuffer& ret_buffer) {
    std::string req_type = Poco::Net::HTTPRequest::HTTP_POST;
    create_and_send_request(req_type, true, query_url, body, ret_buffer);
  }
  // Execute a Get query
  inline void get_by_reference(std::string& query_url, AOSSL::StringBuffer& ret_buffer) {
    std::string body = "";
    std::string req_type = Poco::Net::HTTPRequest::HTTP_GET;
    create_and_send_request(req_type, false, query_url, body, ret_buffer);
  }
  // Execute a Delete query
  inline void delete_by_reference(std::string& query_url, AOSSL::StringBuffer& ret_buffer) {
    std::string body = "";
    std::string req_type = Poco::Net::HTTPRequest::HTTP_DELETE;
    create_and_send_request(req_type, false, query_url, body, ret_buffer);
  }
  // Authenticate with Vault
  inline void authenticate(int auth_type, std::string un, std::string pw) {
    // Post the auth credentials to the correct endpoint based on the auth type
    AOSSL::StringBuffer auth_response;
    std::string auth_url;
    std::string auth_body;
    if (auth_type == BASIC_AUTH_TYPE) {
      auth_url.assign(std::string("/v1/auth/userpass/login/") + un);
      auth_body.assign(std::string("{\"password\": \"") + pw + std::string("\"}"));
    } else {
      auth_url.assign("/v1/auth/approle/login");
      auth_body.assign(std::string("{\"role_id\": \"") + un + \
          std::string("\", \"secret_id\": \"") + pw + std::string("\"}"));
    }
    post_by_reference(auth_url, auth_body, auth_response);
    if (!(auth_response.success)) {throw std::invalid_argument(auth_response.err_msg);}
    // Parse out the returned Vault Auth Token and stuff it in the ret_buffer
    rapidjson::Document d;
    d.Parse<rapidjson::kParseStopWhenDoneFlag>(auth_response.val.c_str());
    if (d.HasParseError()) {
      throw std::invalid_argument(GetParseError_En(d.GetParseError()));
    } else if (d.IsObject()) {
      const rapidjson::Value& token_val = d["auth"]["client_token"];
      vault_token.assign(token_val.GetString());
      is_authenticated = true;
    }
  }
 public:
   //! Construct a secured Vault Administrator
  VaultAdmin(std::string& vaddr, std::string& secrets_path, int tout, \
      std::string& cert, int auth_type, std::string& un, std::string& pw) \
      {init(vaddr, secrets_path, tout, un, pw, auth_type);secured=true;cert_location.assign(cert);}
  //! Construct an unsecured Vault Administrator
  VaultAdmin(std::string& vaddr, std::string& secrets_path, int tout, \
      int auth_type, std::string& un, std::string& pw) \
      {init(vaddr, secrets_path, tout, un, pw, auth_type);}
  ~VaultAdmin() {}
  //! Does a key exist?
  inline bool opt_exist(std::string key) {
    if (!is_authenticated) {
      // Authenticate
      authenticate(authentication_type, username, password);
    }
    // Execute the Vault Request
    StringBuffer secret_buffer;
    std::string request_path = secrets_url_path + key;
    get_by_reference(request_path, secret_buffer);
    if (secret_buffer.success) {
      return true;
    }
    return false;
  }

  //! Get an option by key
  inline StringBuffer* get_opt(std::string key) {
    StringBuffer *buf = new StringBuffer();
    get_opt(key, *buf);
    return buf;
  }

  //! Get an option by key
  inline void get_opt(std::string key, StringBuffer& val) {
    if (!is_authenticated) {
      // Authenticate
      authenticate(authentication_type, username, password);
    }
    // Execute the Vault Request
    std::string request_path = secrets_url_path + key;
    get_by_reference(request_path, val);
  }

  inline void gen_ssl_cert(std::string& role_name, std::string& common_name, \
      SslCertificateBuffer& cert_buf) {
    if (!is_authenticated) {
      // Authenticate
      authenticate(authentication_type, username, password);
    }
    std::string req_body = std::string("{\"common_name\": \"") + common_name + std::string("\"}");
    std::string request_path = std::string("/v1/pki/issue/") + role_name;
    StringBuffer http_response;
    post_by_reference(request_path, req_body, http_response);
    if (!(http_response.success)) {
      cert_buf.success = false;
      cert_buf.err_msg.assign(http_response.err_msg);
    }
    // Parse out the returned Vault Auth Token and stuff it in the ret_buffer
    rapidjson::Document d;
    d.Parse<rapidjson::kParseStopWhenDoneFlag>(http_response.val.c_str());
    if (d.HasParseError()) {
      cert_buf.success = false;
      cert_buf.err_msg.assign(GetParseError_En(d.GetParseError()));
    } else if (d.IsObject()) {
      cert_buf.success = true;
      const rapidjson::Value& cert_val = d["data"]["certificate"];
      cert_buf.certificate.assign(cert_val.GetString());
      const rapidjson::Value& issuing_ca_val = d["data"]["issuing_ca"];
      cert_buf.issuing_ca.assign(issuing_ca_val.GetString());
      const rapidjson::Value& ca_chain_val = d["data"]["ca_chain"];
      cert_buf.ca_chain.assign(ca_chain_val.GetString());
      const rapidjson::Value& private_key_val = d["data"]["private_key"];
      cert_buf.private_key.assign(private_key_val.GetString());
      const rapidjson::Value& private_key_type_val = d["data"]["private_key_type"];
      cert_buf.private_key_type.assign(private_key_type_val.GetString());
      const rapidjson::Value& serial_number_val = d["data"]["serial_number"];
      cert_buf.serial_number.assign(serial_number_val.GetString());
    }
  }

  inline void gen_consul_token(std::string& role_name, StringBuffer& token_buf) {
    if (!is_authenticated) {
      // Authenticate
      authenticate(authentication_type, username, password);
    }
    std::string request_path = std::string("/v1/consul/creds/") + role_name;
    StringBuffer http_response;
    get_by_reference(request_path, http_response);
    if (!(http_response.success)) {
      token_buf.success = false;
      token_buf.err_msg.assign(http_response.err_msg);
    }
    // Parse out the returned Vault Auth Token and stuff it in the ret_buffer
    rapidjson::Document d;
    d.Parse<rapidjson::kParseStopWhenDoneFlag>(http_response.val.c_str());
    if (d.HasParseError()) {
      token_buf.success = false;
      token_buf.err_msg.assign(GetParseError_En(d.GetParseError()));
    } else if (d.IsObject()) {
      const rapidjson::Value& token_val = d["data"]["token"];
      token_buf.val.assign(token_val.GetString());
      token_buf.success = true;
    }
  }

  //! Re-load configuration
  inline void load_config() {}
};

}  // namespace AOSSL

#endif  // AOSSL_VAULT_INCLUDE_VAULT_ADMIN_H_
