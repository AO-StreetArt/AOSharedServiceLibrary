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
#include <vector>

#include "include/service.h"

std::string AOSSL::Service::to_json() const {
  std::string id_key = "ID";
  std::string name_key = "Name";
  std::string tags_key = "Tags";
  std::string addr_key = "Address";
  std::string port_key = "Port";

  // Build the base JSON String
  std::string json_str = "{\"" + id_key + "\": \"" + id + "\", \"" +
  name_key + "\": \"" + name + "\", \"";

  // Add in any service tags
  if (num_tags() > 0) {
    json_str = json_str + tags_key + "\": [\"" + tags[0] + "\"";

    for (int i = 1; i < num_tags(); i++) {
      json_str = json_str + ", \"" + tags[i] + "\"";
    }

    json_str = json_str + "], \"";
  }

  // Finish the base json string
  json_str = json_str + addr_key + "\": \"" + address + "\"" +
  ", \"" + port_key + "\": " + port;

  // Add the Health Check
  if (!check.url.empty()) {
    json_str = json_str + ", \"Check\": {\"HTTP\": \"" + check.url \
        + "\", \"Interval\": \"" + check.interval + "\", \"TTL\": \"" \
        + check.ttl + "\"}";
  }

  json_str = json_str + "}";

  return json_str;
}
