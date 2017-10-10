// Copyright 2017 folly
#ifndef CENG778_PROJECT_INCLUDE_FREQUENCY_HISTOGRAM_PURE_LFU_H
#define CENG778_PROJECT_INCLUDE_FREQUENCY_HISTOGRAM_PURE_LFU_H

#include <unordered_map>
#include <string>
#include "frequency-histogram/frequency_histogram.h"

class PureLfuHistogram : public FrequencyHistogram {
 private:
  std::unordered_map<std::string, std::uint32_t> histogram;

 public:
  std::uint32_t Add(const std::string &) override;
  std::uint32_t Estimate(const std::string &) const override;
};


#endif  // CENG778_PROJECT_FREQUENCY_HISTOGRAM_INCLUDE_PURE_LFU_H
