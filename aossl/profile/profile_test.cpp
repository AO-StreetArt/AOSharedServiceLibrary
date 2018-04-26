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
#include <string>
#include <unistd.h>
#include "include/tiered_app_profile.h"

int main(int argc, char** argv) {
  // Strings
  std::string key1 = "first";
  std::string key2 = "second";
  std::string key3 = "third";
  std::string bad_key = "badkey";
  std::string val1 = "one";
  std::string val2 = "two";
  std::string val3 = "three";
  std::string val4 = "four";

  // Build the profile
  AOSSL::TieredApplicationProfile profile(argc, argv);
  profile.add_opt(key1, val1);
  profile.add_opt(key2, val2);
  assert(profile.opt_exist(key1));
  assert(profile.opt_exist(key2));
  assert(!(profile.opt_exist(bad_key)));
  AOSSL::StringBuffer buf;
  profile.get_opt(key1, buf);
  assert(buf.val == val1);
  AOSSL::StringBuffer buf2;
  profile.get_opt(key2, buf2);
  assert(buf2.val == val2);

  // Set a property file
  profile.set_property_file(std::string("test/test.properties"));
  profile.load_config();
  AOSSL::StringBuffer buf3;
  profile.get_opt(key1, buf3);
  assert(buf3.val == val3);
  AOSSL::StringBuffer buf4;
  profile.get_opt(key2, buf4);
  assert(buf4.val == val4);
}
