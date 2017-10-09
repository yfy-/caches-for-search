// Copyright 2017 folly
#include <string>
#include "frequency-histogram/pure_lfu_histogram.h"

std::uint64_t PureLfuHistogram::Add(const std::string &query) {
  if (histogram.find(query) != histogram.end()) {
    histogram[query]++;
  } else {
    histogram[query] = 1;
  }

  return histogram[query];
}

std::uint64_t PureLfuHistogram::Estimate(const std::string &query) const {
  return histogram.at(query);
}
