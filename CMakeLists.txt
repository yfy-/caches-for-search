cmake_minimum_required(VERSION 3.7)
project(ceng778_project)

set(CMAKE_CXX_STANDARD 11)
set(INC ./include)

include_directories(${INC})

set(SOURCE_FILES src/main.cpp src/lfu_cache.cpp ${INC}/lfu_cache.h
        ${INC}/frequency_histogram.h src/frequency-histogram/pure_lfu_histogram.cpp
        ${INC}/pure_lfu_histogram.h src/util.cpp ${INC}/util.h
        ${INC}/spectral_bloom_filter.h ${INC}/fnv1a.h src/hash/fnv1a.cpp
        ${INC}/murmur3.h src/hash/murmur3.c ${INC}/tiny_lfu_histogram.h
        src/frequency-histogram/wlfu_histogram.cpp ${INC}/wlfu_histogram.h)

add_executable(ceng778_project ${SOURCE_FILES})
