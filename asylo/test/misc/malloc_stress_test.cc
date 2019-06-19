/*
 *
 * Copyright 2017 Asylo authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "absl/strings/str_cat.h"
#include "asylo/util/logging.h"
#include "asylo/util/binary_search.h"

namespace asylo {
namespace {

constexpr size_t kNumThreads = 5;
constexpr size_t kAllocations = 100;
constexpr size_t kAllocationSize = 10000;

// Return the largest malloc which succeeds, using binary search
size_t LargestSuccessfulMalloc() {
  auto malloc_succeeds = [](size_t size) {
    void *ptr = malloc(size);
    if (ptr != NULL) {
      free(ptr);
      return true;
    }
    return false;
  };
  return BinarySearch(malloc_succeeds);
}

void *MallocStress(void *) {
  void *mem[kAllocations];
  for (int i = 0; i < kAllocations; ++i) {
    mem[i] = malloc(kAllocationSize);
  }
  for (int i = 0; i < kAllocations; ++i) {
    free(mem[i]);
  }

  return nullptr;
}

void LogBadAlloc(const std::bad_alloc &e, void *brk_start) {
  LOG(ERROR) << "Failed to allocate with malloc: " << e.what() << std::endl
             << "Total memory allocated (using sbrk subtraction) is: "
             << (size_t)sbrk(0) - (size_t)brk_start << " bytes" << std::endl
             << "Largest malloc that succeeds at the moment is: "
             << LargestSuccessfulMalloc() << " bytes";
}

// Creates kNumThreads that run |MallocStress| and waits for all threads to
// join.
TEST(MallocTest, EnclaveMalloc) {
  LOG(INFO) << "Largest malloc that succeeds at test start is: "
            << LargestSuccessfulMalloc() << " bytes";
  pthread_t threads[kNumThreads];
  // Keep track of the break pointer at the start of the malloc stress test.
  // This can be used to calculate total allocated heap memory.
  void *brk_start = sbrk(0);
  try {
    for (int i = 0; i < kNumThreads; ++i) {
      ASSERT_EQ(pthread_create(&threads[i], nullptr, &MallocStress, nullptr),
                0);
    }
  } catch (const std::bad_alloc &e) {
    LOG(ERROR) << "Caught bad_alloc during pthread_create";
    LogBadAlloc(e, brk_start);
    FAIL();
  }
  try {
    for (int i = 0; i < kNumThreads; ++i) {
      ASSERT_EQ(pthread_join(threads[i], nullptr), 0);
    }
  } catch (const std::bad_alloc &e) {
    LOG(ERROR) << "Caught bad_alloc during pthread_join";
    LogBadAlloc(e, brk_start);
    FAIL();
  }
}

}  // namespace
}  // namespace asylo
