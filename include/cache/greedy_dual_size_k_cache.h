//
// Created by yfy on 10/22/17.
//

#ifndef CENG778_PROJECT_GREEDY_DUAL_SIZE_K_CACHE_H
#define CENG778_PROJECT_GREEDY_DUAL_SIZE_K_CACHE_H

#include <cmath>
#include <unordered_map>
#include "cache/cache.h"
#include "cache/node_doubly_linked_list.h"

class GreedyDualSizeKCache : public Cache {
 private:
  NodeDoublyLinkedList* cache_;
  std::unordered_map<std::string, NodeDoublyLinkedList::Node*> cache_table_;
  std::uint32_t offset_;
  std::uint32_t k_;

  void PutBeforeHigherHValue(NodeDoublyLinkedList::Node*);

  inline std::uint32_t HValue(std::uint32_t freq) {
    return static_cast<std::uint32_t>(pow(freq, k_)) + offset_;
  }

 public:
  GreedyDualSizeKCache(std::uint32_t, std::uint32_t, FrequencyHistogram*);

  bool IsExist(const std::string&) override;
  std::string ToString() const override;
  virtual ~GreedyDualSizeKCache();
};

#endif //CENG778_PROJECT_GREEDY_DUAL_SIZE_K_CACHE_H
