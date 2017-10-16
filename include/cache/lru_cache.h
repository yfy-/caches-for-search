//
// Created by yfy on 10/15/17.
//

#ifndef CENG778_PROJECT_LRU_CACHE_H_H
#define CENG778_PROJECT_LRU_CACHE_H_H

#include <unordered_map>
#include "cache.h"
#include "NodeDoublyLinkedList.h"

class LruCache : public Cache {
 public:
  LruCache(uint32_t s);
  bool IsExist(const std::string&) override;
  inline void SetFrequencyHistogram(FrequencyHistogram* fh) { freq_hist_ = fh; }
  std::string ToString() const override;
  virtual ~LruCache();

 private:
  std::unordered_map<std::string, NodeDoublyLinkedList::Node*> cache_table_;
  NodeDoublyLinkedList* cache_;
  bool IsExistWithFreqHist(const std::string &query);
};

#endif //CENG778_PROJECT_LRU_CACHE_H_H
