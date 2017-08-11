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

// A slot pool is a thread-safe container for retrieving values from a fixed
// set of integers.  This is an internal component for any resource pool, such
// as a connection pool.

#ifndef AOSSL_CORE_SLOT_POOL_H_
#define AOSSL_CORE_SLOT_POOL_H_

#include <mutex>

namespace AOSSL {

class SlotPool {

  // Internal integers
  int connection_limit = 1;

  // Mutex for finding a next slot
  std::mutex get_slot_mutex;

  // Array of ints (0/1) which determine which connections are open vs closed
  int *slots;

public:
  // Create a new Resource pool with the given limit
  inline SlotPool(int conn_limit) {
    connection_limit = conn_limit;
    slots = new int[connection_limit];
    for (int i = 0; i < connection_limit; i++) {
      slots[i] = 0;
    }
  }

  // Destroy the resource pool
  inline ~SlotPool() {
    delete[] slots;
  }

  // Retrieve the next available resource slot
  inline int find_next_slot() {
    // Get the mutex to ensure we get a unique connection
    std::lock_guard<std::mutex> lock(get_slot_mutex);

    // Find the next available connection slot
    // If none are available, wait until one is freed
    bool connection_found = false;
    while (!connection_found) {
      for (int j = 0; j < connection_limit; j++) {
        if (slots[j] == 0) {
          // slot is available
          slots[j] = 1;
          return j;
        }
      }
    }
    return -1;
  }

  // Release a slot
  inline void release_slot(int slot_index) {
    slots[slot_index] = 0;
  }
};

}
#endif  // AOSSL_CORE_SLOT_POOL_H_
