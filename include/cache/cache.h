//
// Created by yfy on 10/9/17.
//

#ifndef CENG778_PROJECT_CACHE_H
#define CENG778_PROJECT_CACHE_H

#include <string>
#include "frequency-histogram/frequency_histogram.h"

class Cache {

 public:
  explicit Cache(std::uint32_t s) : size_{s}, count_{0}, freq_hist_{nullptr} {}
  inline std::uint32_t Size() const { return size_; }
  inline std::uint32_t Count() const { return count_; }
  inline FrequencyHistogram* GetFreqHist_() const {
    return freq_hist_;
  }
  inline void SetFrequencyHistogram(FrequencyHistogram* fh) { freq_hist_ = fh; }
  virtual bool IsExist(const std::string&) = 0;
  virtual std::string ToString() const = 0;

 protected:
  std::uint32_t size_;
  std::uint32_t count_;

  FrequencyHistogram* freq_hist_;
};

#endif //CENG778_PROJECT_CACHE_H
