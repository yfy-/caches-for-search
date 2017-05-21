//
// Created by yfy on 5/18/17.
//

#ifndef CENG778_PROJECT_MURMUR3_H
#define CENG778_PROJECT_MURMUR3_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Use second 8 bytes of out
void MurmurHash3_x64_128(const void *key, const uint32_t len, uint32_t seed, void *out);

#ifdef __cplusplus
}
#endif

#endif //CENG778_PROJECT_MURMUR3_H
