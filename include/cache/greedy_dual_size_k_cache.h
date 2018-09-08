//
// Created by yfy on 10/22/17.
//

#ifndef CENG778_PROJECT_GREEDY_DUAL_SIZE_K_CACHE_H
#define CENG778_PROJECT_GREEDY_DUAL_SIZE_K_CACHE_H

#include <cmath>
#include <unordered_map>
#include <iostream>
#include "cache/cache.h"
#include "cache/node_doubly_linked_list.h"

class GreedyDualSizeKCache : public Cache {
 private:
  NodeDoublyLinkedList* cache_;
  std::unordered_map<std::string, NodeDoublyLinkedList::Node*> cache_table_;
  float offset_; // Aging factor
  std::uint32_t k_;

  static constexpr float estimated_freq_[20] = {
    0.15, 0.66, 1.53, 2.47, 3.48, 4.47, 5.4, 6.43, 7.37, 8.32, 9.54, 10.26,
    11.2, 12.13, 13.27, 13.74, 15.03, 17.25, 16.85, 17.89
  };

  bool InMemoryGdsfk(const std::string&);
  void PutBeforeHigherHValue(NodeDoublyLinkedList::Node*);

  inline float HValue(std::uint32_t freq) {
    float estimated = freq > 20 ? static_cast<float>(freq) :
        estimated_freq_[freq - 1];
    return estimated + offset_;
  }

 public:
  GreedyDualSizeKCache(std::uint32_t, std::uint32_t);

  bool IsExist(const std::string&) override;
  std::string ToString() const override;
  virtual ~GreedyDualSizeKCache();
};

#endif //CENG778_PROJECT_GREEDY_DUAL_SIZE_K_CACHE_H
