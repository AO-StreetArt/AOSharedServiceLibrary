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

// Intentionally do some funky initialization to ensure that the
// string values, if passed without being wrapped, would go out of scope.
// Because we wrap it in a manually-allocated buffer, we expect it to stay
// in=scope

#include <assert.h>
#include <string>
#include "include/buffers.h"

AOSSL::StringBuffer *buf = NULL;
AOSSL::KvBuffer *kv_buf = NULL;

void init_buffers() {
  buf = new AOSSL::StringBuffer;
  kv_buf = new AOSSL::KvBuffer;
}

void build_standard_buffer() {
  buf->success = false;
  std::string err_str = "No Error";
  buf->err_msg = err_str;
  std::string val_str = "Test Value";
  buf->val = val_str;
}

void build_kv_buffer() {
  kv_buf->success = false;
  std::string err_str = "No Error";
  kv_buf->err_msg = err_str;
  std::string val_str = "Test Value";
  kv_buf->val = val_str;
  std::string key_str = "Test Key";
  kv_buf->key = key_str;
}

void test_standard_buffer() {
  assert(buf->success == false);
  assert(buf->err_msg == "No Error");
  assert(buf->val == "Test Value");
}

void test_kv_buffer() {
  assert(kv_buf->success == false);
  assert(kv_buf->err_msg == "No Error");
  assert(kv_buf->val == "Test Value");
  assert(kv_buf->key == "Test Key");
}

int main() {
  init_buffers();
  build_standard_buffer();
  build_kv_buffer();
  test_standard_buffer();
  test_kv_buffer();
  delete buf;
  delete kv_buf;
}
