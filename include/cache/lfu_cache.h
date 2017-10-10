// Copyright 2017 folly
#ifndef CENG778_PROJECT_INCLUDE_LFU_CACHE_H
#define CENG778_PROJECT_INCLUDE_LFU_CACHE_H

#include <string>
#include <unordered_map>
#include <utility>
#include "frequency-histogram/frequency_histogram.h"
#include "cache/cache.h"
#include "cache/NodeDoublyLinkedList.h"

class LfuCache : public Cache {
 private:

  NodeDoublyLinkedList::Node* head_;
  std::unordered_map<std::string, NodeDoublyLinkedList::Node*> cache_table_;

  void FindPlace(NodeDoublyLinkedList::Node*, std::uint32_t);

 public:

  bool IsExist(const std::string&) override;
  std::string ToString();

  LfuCache(std::uint32_t , FrequencyHistogram*);

  virtual ~LfuCache();
};


#endif  // CENG778_PROJECT_INCLUDE_LFU_CACHE_H
