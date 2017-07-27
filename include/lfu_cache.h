// Copyright 2017 folly
#ifndef CENG778_PROJECT_INCLUDE_LFU_CACHE_H
#define CENG778_PROJECT_INCLUDE_LFU_CACHE_H

#include <frequency_histogram.h>
#include <string>
#include <unordered_map>

class LfuCache {
 private:
  struct Node {
    std::string data;
    Node* next;
    explicit Node(std::string d) : data{d}, next{nullptr} {}
  };

  std::uint64_t size_;
  std::uint64_t count_;

  Node* head_;
  std::unordered_map<std::string, Node*> cache_;
  FrequencyHistogram* freq_hist_;

  void FindPlace(Node*, std::uint64_t);

 public:
  inline std::uint64_t Size() const { return size_; }
  inline std::uint64_t Count() const { return count_; }
  bool IsExist(const std::string&);
  std::string ToString();

  LfuCache(std::uint64_t , FrequencyHistogram*);

  virtual ~LfuCache();
};


#endif  // CENG778_PROJECT_INCLUDE_LFU_CACHE_H
