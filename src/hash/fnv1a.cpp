// Copyright 2017 folly

#include <string>
#include "hash/fnv1a.h"

// FNV constants for 64 bit range
const std::uint64_t kFnvPrime = 1099511628211U;
const std::uint64_t kOffsetBasis = 14695981039346656037U;


std::uint64_t Fnv1a(const std::string &kKey) {
  std::uint64_t hash = kOffsetBasis;

  const char* kPtrKey = kKey.c_str();
  for (int i = 0; i < kKey.length(); i++) {
    hash ^= *(kPtrKey + i);
    hash *= kFnvPrime;
  }

  return hash;
}

