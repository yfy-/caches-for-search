// Copyright 2017 folly

#ifndef CENG778_PROJECT_INCLUDE_MURMUR3_H
#define CENG778_PROJECT_INCLUDE_MURMUR3_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Use second 8 bytes of out
void MurmurHash3_x64_128(const void*, const uint32_t, uint32_t, void*);

#ifdef __cplusplus
}
#endif

#endif  // CENG778_PROJECT_INCLUDE_MURMUR3_H
