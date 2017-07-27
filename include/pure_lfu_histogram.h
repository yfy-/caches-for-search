// Copyright 2017 folly
#ifndef CENG778_PROJECT_INCLUDE_PURE_LFU_H
#define CENG778_PROJECT_INCLUDE_PURE_LFU_H

#include <frequency_histogram.h>
#include <unordered_map>
#include <string>

class PureLfuHistogram : public FrequencyHistogram {
 private:
  std::unordered_map<std::string, std::uint64_t> histogram;

 public:
  std::uint64_t Add(const std::string &) override;
  std::uint64_t Estimate(const std::string &) const override;
};


#endif  // CENG778_PROJECT_INCLUDE_PURE_LFU_H
