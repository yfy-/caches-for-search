//
// Created by yfy on 10/9/17.
//

#ifndef CENG778_PROJECT_CACHE_H
#define CENG778_PROJECT_CACHE_H

#include <string>
#include "frequency-histogram/frequency_histogram.h"

class Cache {

 public:
  struct Node {
    std::string data;
    Node* next;
    explicit Node(const std::string &d) : data{d}, next{nullptr} {}
  };

  explicit Cache(std::uint64_t s) : size_{s}, count_{0}, freq_hist_{nullptr} {}
  inline std::uint64_t Size() const { return size_; }
  inline std::uint64_t Count() const { return count_; }
  virtual bool IsExist(const std::string&) = 0;

 protected:
  std::uint64_t size_;
  std::uint64_t count_;

  FrequencyHistogram* freq_hist_;
};

#endif //CENG778_PROJECT_CACHE_H
