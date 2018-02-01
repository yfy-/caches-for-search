// Copyright 2017 folly
#include <string>
#include "frequency-histogram/pure_lfu_histogram.h"

std::uint32_t PureLfuHistogram::Add(const std::string &query) {
  histogram[query]++;
  return histogram.at(query);
}

std::uint32_t PureLfuHistogram::Estimate(const std::string &query) const {
  if (histogram.find(query) != histogram.end())
    return histogram.at(query);

  return 0;
}
std::string PureLfuHistogram::ToString() const {
  return "";
}
