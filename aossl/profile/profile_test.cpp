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

#include <assert.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>

#include "include/tiered_app_profile.h"

int main(int argc, char** argv) {
  // Strings
  std::string key1 = "kone";
  std::string key2 = "ktwo";
  std::string key3 = "first";
  std::string key4 = "second";
  std::string ckey1 = "test/prof/kone";
  std::string ckey2 = "test/prof/ktwo";
  std::string bad_key = "badkey";
  std::string val1 = "one";
  std::string val2 = "two";
  std::string val3 = "three";
  std::string val4 = "four";
  std::string val5 = "five";
  std::string val6 = "six";
  std::string val7 = "seven";
  std::string val8 = "eight";

  // Vector-arg profile tests
  std::string akey = "test";
  std::string avalue = "1";
  std::vector<std::string> args;
  args.push_back(std::string("test=1"));
  AOSSL::TieredApplicationProfile basic_profile(args, \
    std::string("test2"), std::string("prof2"));
  basic_profile.add_opt(akey, avalue);
  basic_profile.load_config();

  // Basic Profile Tests
  AOSSL::TieredApplicationProfile profile(argc, argv, \
      std::string("test"), std::string("prof"));
  profile.add_opt(key1, val1);
  profile.add_opt(key2, val2);
  profile.add_opt(key3, val1);
  profile.add_opt(key4, val2);
  assert(profile.opt_exist(key1));
  assert(profile.opt_exist(key2));
  assert(profile.opt_exist(key3));
  assert(profile.opt_exist(key4));
  assert(!(profile.opt_exist(bad_key)));
  AOSSL::StringBuffer buf;
  profile.get_opt(key1, buf);
  std::cout << buf.val << std::endl;
  assert(buf.val == val1);
  AOSSL::StringBuffer buf2;
  profile.get_opt(key2, buf2);
  std::cout << buf2.val << std::endl;
  assert(buf2.val == val2);

  // Property File Tests
  std::string props_file_name = "test/test.properties";
  profile.set_property_file(props_file_name);
  profile.load_config();
  AOSSL::StringBuffer buf3;
  profile.get_opt(key1, buf3);
  std::cout << buf3.val << std::endl;
  assert(buf3.val == val3);
  AOSSL::StringBuffer buf4;
  profile.get_opt(key2, buf4);
  std::cout << buf4.val << std::endl;
  assert(buf4.val == val4);

  // Consul Tests
  profile.set_consul_address(std::string("http://127.0.0.1:8500"));
  profile.load_config();
  AOSSL::StringBuffer buf5;
  profile.get_opt(key1, buf5);
  std::cout << buf5.val << std::endl;
  assert(buf5.val == val5);
  AOSSL::StringBuffer buf6;
  profile.get_opt(key2, buf6);
  std::cout << buf6.val << std::endl;
  assert(buf6.val == val6);

  // Vault tests
  std::string vaddr("http://127.0.0.1:8200");
  std::string secrets_path("/v1/secret/data/");
  std::string un("test");
  std::string pw("test");
  profile.set_vault_address(vaddr, secrets_path, 5, 1, un, pw);
  std::string secretKey("testKey");
  std::string secretVal("testValue");
  profile.add_secure_opt(secretKey);
  profile.load_config();
  AOSSL::StringBuffer buf7;
  profile.get_opt(secretKey, buf7);
  if (buf7.success) {
    std::cout << buf7.val << std::endl;
    assert(buf7.val == secretVal);
  } else {
    std::cout << buf7.err_msg << std::endl;
    assert(false);
  }

  // Initialization logic tests
  std::vector<std::string> cli_args;
  cli_args.push_back(std::string("test=1"));
  cli_args.push_back(std::string("vault=http://127.0.0.1:8200"));
  cli_args.push_back(std::string("vault.authtype=BASIC"));
  cli_args.push_back(std::string("vault.un=test"));
  cli_args.push_back(std::string("vault.pw=test"));
  cli_args.push_back(std::string("consul=http://127.0.0.1:8500"));
  cli_args.push_back(std::string("props=test/test.properties"));
  AOSSL::TieredApplicationProfile startup_profile(cli_args, \
    std::string("test3"), std::string("prof3"));
  startup_profile.add_opt(akey, avalue);
  startup_profile.add_opt(key1, val1);
  startup_profile.add_opt(key2, val2);
  startup_profile.add_opt(key3, val1);
  startup_profile.add_opt(key4, val2);
  startup_profile.add_secure_opt(secretKey);
  startup_profile.load_config();
  // Make sure all the correct keys are still present
  assert(startup_profile.opt_exist(secretKey));
  assert(startup_profile.opt_exist(akey));
  assert(startup_profile.opt_exist(key1));
  assert(startup_profile.opt_exist(key2));
  assert(startup_profile.opt_exist(key3));
  assert(startup_profile.opt_exist(key4));
  // Compare the config values to make sure
  // they're picked up in the correct order
  AOSSL::StringBuffer stbuf;
  startup_profile.get_opt(secretKey, stbuf);
  std::cout << stbuf.val << std::endl;
  assert(stbuf.val == secretVal);
  AOSSL::StringBuffer stbuf2;
  startup_profile.get_opt(key1, stbuf2);
  std::cout << stbuf2.val << std::endl;
  assert(stbuf2.val == val5);
  AOSSL::StringBuffer stbuf3;
  startup_profile.get_opt(key2, stbuf3);
  std::cout << stbuf3.val << std::endl;
  assert(stbuf3.val == val6);
  AOSSL::StringBuffer stbuf4;
  startup_profile.get_opt(akey, stbuf4);
  std::cout << stbuf4.val << std::endl;
  assert(stbuf4.val == avalue);
}
