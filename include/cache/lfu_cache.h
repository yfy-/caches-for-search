// Copyright 2017 folly
#ifndef CENG778_PROJECT_INCLUDE_LFU_CACHE_H
#define CENG778_PROJECT_INCLUDE_LFU_CACHE_H

#include <string>
#include <unordered_map>
#include <utility>
#include "frequency-histogram/frequency_histogram.h"
#include "cache/cache.h"

class LfuCache : public Cache {
 private:

  Cache::Node* head_;
  std::unordered_map<std::string, Cache::Node*> cache_table_;

  void FindPlace(Cache::Node*, std::uint64_t);

 public:

  bool IsExist(const std::string&) override;
  std::string ToString();

  LfuCache(std::uint64_t, FrequencyHistogram*);

  virtual ~LfuCache();
};


#endif  // CENG778_PROJECT_INCLUDE_LFU_CACHE_H
