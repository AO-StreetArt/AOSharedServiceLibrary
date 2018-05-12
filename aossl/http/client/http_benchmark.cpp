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

#include <hayai/hayai.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>

#include "include/http_interface.h"
#include "include/factory_http_client.h"
AOSSL::HttpInterface *http;

// Benchmarks

BENCHMARK(HTTP, Get, 10, 100) {
  // Send the request
  std::string url = "http://httpbin.org/get";
  std::string ret_val = http->get(url, 5);
  if (ret_val.empty()) {
    std::cout <<"Get Request Failed" << std::endl;
  }
}

BENCHMARK(HTTP, Put, 10, 100) {
  // Send the request
  std::string url = "http://httpbin.org/put";
  bool success = http->put(url, "123", 5);
  if (!success) {
    // We now have the full response
    std::cout <<"Put Request Failed" << std::endl;
  }
}

BENCHMARK(HTTP, Post, 10, 100) {
  // Send the request
  std::string url = "http://httpbin.org/post";
  bool succ = http->post(url, "CLYMAN", 5);
  if (!succ) {
    // We now have the full response
    std::cout <<"Post Request Failed" << std::endl;
  }
}

BENCHMARK(HTTP, Delete, 10, 100) {
  // Send the request
  std::string url = "http://httpbin.org/delete";
  bool success = http->del(url, 5);
  if (!success) {
    // We now have the full response
    std::cout <<"Delete Request Failed" << std::endl;
  }
}

// Main Method

int main() {
  AOSSL::HttpClientFactory http_client_factory;

  // Set up internal variables
  std::cout << "Internal Logging Intialized" <<std::endl;

  // Set up HTTP Client
  http = http_client_factory.get_http_interface();
  std::cout << "HTTP Outbound Interface Created" << std::endl;

  // Run Tests

  hayai::ConsoleOutputter consoleOutputter;

  hayai::Benchmarker::AddOutputter(consoleOutputter);
  hayai::Benchmarker::RunAllTests();

  // Post-Test Teardown

  delete http;
  return 0;
}
