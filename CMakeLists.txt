cmake_minimum_required(VERSION 3.7)
project(ceng778_project)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp src/lfu_cache.cpp src/lfu_cache.h
        src/frequency-histogram/frequency_histogram.h src/frequency-histogram/pure_lfu_histogram.cpp
        src/frequency-histogram/pure_lfu_histogram.h src/util.cpp src/util.h
        src/frequency-histogram/spectral_bloom_filter.h src/hash/fnv1a.h src/hash/fnv1a.cpp
        src/hash/murmur3.h src/hash/murmur3.c src/frequency-histogram/tiny_lfu_histogram.h)
add_executable(ceng778_project ${SOURCE_FILES})