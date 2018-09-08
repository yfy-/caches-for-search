cmake_minimum_required(VERSION 3.7)
project(ceng778_project)

set(CMAKE_CXX_STANDARD 11)
set(INC ./include)

include_directories(${INC})
find_package(Threads)

set (SOURCE_FILES src/main.cpp ${INC}/doubly_linked_list.h
  ${INC}/policy/replacement/replacement_policy.h
  ${INC}/policy/replacement/only_eviction_replacement_policy.h
  src/policy/replacement/only_eviction_replacement_policy.cpp
  ${INC}/policy/eviction/eviction_policy.h
  ${INC}/policy/admission/admission_policy.h
  ${INC}/policy/eviction/lru_eviction.h
  src/policy/eviction/lru_eviction.cpp
  ${INC}/policy/eviction/inmemory_lfu_eviction.h
  src/policy/eviction/inmemory_lfu_eviction.cpp
  ${INC}/policy/eviction/lfu_eviction.h
  src/policy/eviction/lfu_eviction.cpp
  ${INC}/policy/admission/frequency_admission.h
  ${INC}/policy/replacement/admission_replacement_policy.h
  src/policy/replacement/admission_replacement_policy.cpp
  src/frequency-histogram/pure_lfu_histogram.cpp
  ${INC}/frequency-histogram/pure_lfu_histogram.h
  ${INC}/hitrate_evaluation.h
  src/hitrate_evaluation.cpp
  ${INC}/policy/replacement/admission_controlled_replacement_policy.h
  src/policy/replacement/admission_controlled_replacement_policy.cpp
  ${INC}/policy/replacement/static_dynamic_replacement_policy.h
  src/policy/replacement/static_dynamic_replacement_policy.cpp
  src/frequency-histogram/window_lfu_histogram.cpp
  ${INC}/frequency-histogram/window_lfu_histogram.h
  ${INC}/policy/eviction/inmemory_gdsfk.h
  src/policy/eviction/inmemory_gdsfk.cpp
  ${INC}/policy/eviction/gdsfk.h
  src/policy/eviction/gdsfk.cpp
  ${INC}/frequency-histogram/tiny_lfu_histogram.h
  ${INC}/hash/fnv1a.h ${INC}/hash/murmur3.h src/hash/fnv1a.cpp
  src/hash/murmur3.c)

add_executable(ceng778_project ${SOURCE_FILES})
target_link_libraries(ceng778_project ${CMAKE_THREAD_LIBS_INIT})
