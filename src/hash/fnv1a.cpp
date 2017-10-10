// Copyright 2017 folly

#include <string>
#include "hash/fnv1a.h"

// FNV constants for 64 bit range
const std::uint32_t kFnvPrime = 16777619;
const std::uint32_t kOffsetBasis = 2166136261;

std::uint32_t Fnv1a(const std::string &kKey) {
  std::uint32_t hash = kOffsetBasis;

  const char* kPtrKey = kKey.c_str();
  for (int i = 0; i < kKey.length(); i++) {
    hash ^= *(kPtrKey + i);
    hash *= kFnvPrime;
  }

  return hash;
}

