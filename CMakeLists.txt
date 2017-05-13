cmake_minimum_required(VERSION 3.7)
project(ceng778_project)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp lfu_cache.cpp lfu_cache.h types.h frequency_histogram.h pure_lfu_histogram.cpp pure_lfu_histogram.h util.cpp util.h)
add_executable(ceng778_project ${SOURCE_FILES})