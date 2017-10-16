// Copyright 2017 folly
#ifndef CENG778_PROJECT_INCLUDE_FREQUENCY_HISTOGRAM_FREQUENCY_HISTOGRAM_H
#define CENG778_PROJECT_INCLUDE_FREQUENCY_HISTOGRAM_FREQUENCY_HISTOGRAM_H

#include <string>

class FrequencyHistogram {
 public:
  virtual std::uint32_t Add(const std::string&) = 0;
  virtual std::uint32_t Estimate(const std::string&) const = 0;
  virtual std::string ToString() const = 0;
};


#endif  // CENG778_PROJECT_INCLUDE_FREQUENCY_HISTOGRAM_FREQUENCY_HISTOGRAM_H
