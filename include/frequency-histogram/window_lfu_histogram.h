//
// Created by yfy on 10/28/17.
//

#ifndef CENG778_PROJECT_WINDOW_LFU_HISTOGRAM_H
#define CENG778_PROJECT_WINDOW_LFU_HISTOGRAM_H

#include <unordered_map>
#include <string>
#include "cache/node_doubly_linked_list.h"
#include "frequency-histogram/frequency_histogram.h"

class WindowLfuHistogram : public FrequencyHistogram {

 public:
  explicit WindowLfuHistogram(std::uint32_t);
  virtual ~WindowLfuHistogram();

  uint32_t Add(const std::string &) override;
  uint32_t Estimate(const std::string &string) const override;
  std::string ToString() const override;

 private:
  std::uint32_t window_;
  std::uint32_t count_;
  NodeDoublyLinkedList* query_order_;
  std::unordered_map<std::string, std::uint32_t> histogram_;
};

#endif //CENG778_PROJECT_WINDOW_LFU_HISTOGRAM_H
