//
// Created by yfy on 10/9/17.
//

#ifndef CENG778_PROJECT_RANDOM_CACHE_H
#define CENG778_PROJECT_RANDOM_CACHE_H

#include <unordered_map>
#include <string>
#include <vector>
#include "cache/cache.h"
#include "cache/NodeDoublyLinkedList.h"

class RandomCache : public Cache {
 public:
  explicit RandomCache(uint32_t s);
  bool IsExist(const std::string&) override;
  inline void SetFrequencyHistogram(FrequencyHistogram* fh) { freq_hist_ = fh; }
  virtual inline std::string ToString() const override { return ""; }
  virtual ~RandomCache();

 private:
  std::unordered_map<std::string, NodeDoublyLinkedList::Node*> cache_table_;
  NodeDoublyLinkedList::Node** cache_;

  bool IsExistWithFreqHist(const std::string&);
  void AddNewEntry(const std::string &);
  void ReplaceVictim(const std::string &, NodeDoublyLinkedList::Node*);

};

#endif //CENG778_PROJECT_RANDOM_CACHE_H
