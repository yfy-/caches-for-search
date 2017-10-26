cmake_minimum_required(VERSION 3.7)
project(ceng778_project)

set(CMAKE_CXX_STANDARD 11)
set(INC ./include)

include_directories(${INC})

set(SOURCE_FILES src/main.cpp src/cache/lfu_cache.cpp ${INC}/cache/lfu_cache.h
        ${INC}/frequency-histogram/frequency_histogram.h src/frequency-histogram/pure_lfu_histogram.cpp
        ${INC}/frequency-histogram/pure_lfu_histogram.h src/util.cpp ${INC}/util.h
        ${INC}/spectral_bloom_filter.h ${INC}/hash/fnv1a.h src/hash/fnv1a.cpp
        ${INC}/hash/murmur3.h src/hash/murmur3.c ${INC}/frequency-histogram/tiny_lfu_histogram.h
        ${INC}/frequency-histogram/strawman_histogram.h src/cache/random_cache.cpp
        ${INC}/cache/random_cache.h ${INC}/cache/cache.h src/cache/node_doubly_linked_list.cpp
        ${INC}/cache/node_doubly_linked_list.h include/cache/lru_cache.h src/cache/lru_cache.cpp include/cache/greedy_dual_size_k_cache.h src/cache/greedy_dual_size_k_cache.cpp)

add_executable(ceng778_project ${SOURCE_FILES})
