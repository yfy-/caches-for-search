// Copyright 2017 folly
#ifndef CENG778_PROJECT_INCLUDE_LFU_CACHE_H
#define CENG778_PROJECT_INCLUDE_LFU_CACHE_H

#include <string>
#include <unordered_map>
#include <utility>
#include "frequency-histogram/frequency_histogram.h"
#include "cache/cache.h"
#include "cache/node_doubly_linked_list.h"

class LfuCache : public Cache {
 private:

  NodeDoublyLinkedList* cache_;
  std::unordered_map<std::string, NodeDoublyLinkedList::Node*> cache_table_;
  void PutBeforeMoreFrequent(NodeDoublyLinkedList::Node*);
  bool InMemoryLfu(const std::string&);

 public:

  bool IsExist(const std::string&) override;
  std::string ToString() const override;

  LfuCache(std::uint32_t);
  virtual ~LfuCache();
};


#endif  // CENG778_PROJECT_INCLUDE_LFU_CACHE_H
