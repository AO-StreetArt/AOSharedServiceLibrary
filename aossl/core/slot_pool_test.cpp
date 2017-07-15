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
#include "include/slot_pool.h"

int main() {
  AOSSL::SlotPool *sp = new AOSSL::SlotPool(5);
  int first_slot = sp->find_next_slot();
  int second_slot = sp->find_next_slot();
  int third_slot = sp->find_next_slot();
  assert(first_slot == 0);
  assert(second_slot == 1);
  assert(third_slot == 2);
  sp->release_slot(second_slot);
  int fourth_slot = sp->find_next_slot();
  assert(fourth_slot == 1);
  int fifth_slot = sp->find_next_slot();
  assert(fifth_slot == 3);
  sp->release_slot(first_slot);
  sp->release_slot(fourth_slot);
  sp->release_slot(third_slot);
  sp->release_slot(fifth_slot);
  delete sp;
}
