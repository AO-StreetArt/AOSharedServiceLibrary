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

// Consul Admin Tests

#include <string.h>
#include <assert.h>
#include <string>
#include <iostream>
#include "include/vault_admin.h"
#include "aossl/core/include/buffers.h"

int main() {
  // Build the Vault admin
  std::string vault_address = "http://127.0.0.1:8200";
  std::string secrets_url = "/v1/secret/";
  std::string secret_key = "testKey";
  std::string un = "test";
  std::string pw = "test";
  AOSSL::VaultAdmin vault(vault_address, secrets_url, 5, AOSSL::BASIC_AUTH_TYPE, un, pw);
  // Get the test buffer
  std::cout << "Sending Request" << std::endl;
  AOSSL::StringBuffer buf;
  vault.get_opt(secret_key, buf);
  if (!(buf.success)) {
    std::cout << "Error Retrieving secret: " << buf.err_msg << std::endl;
    assert(false);
  }
  std::cout << buf.val << std::endl;
  // Parse out the data and compare it
  std::string data;
  rapidjson::Document d;
  d.Parse<rapidjson::kParseStopWhenDoneFlag>(buf.val.c_str());
  if (d.HasParseError()) {
    assert(false);
  }
  if (d.IsObject()) {
    const rapidjson::Value& token_val = d["data"]["testKey"];
    data.assign(token_val.GetString());
    std::cout << data << std::endl;
    assert(data == std::string("testValue"));
  } else {
    assert(false);
  }
}
