// Created by yfy on 10/28/17

#include "frequency-histogram/window_lfu_histogram.h"

WindowLfuHistogram::WindowLfuHistogram(std::uint32_t w) : window_{w}, count_{0}
{
  query_order_ = new DoublyLinkedList<std::string>();
}

WindowLfuHistogram::~WindowLfuHistogram() {
  delete query_order_;
}

std::uint32_t WindowLfuHistogram::Add(const std::string &query) {

  if (count_ == window_) {
    DoublyLinkedList<std::string>::Node* forget = query_order_->head_->next;

    query_order_->Disconnect(forget);
    std::string forget_query = forget->content;
    histogram_[forget_query]--;

    if (histogram_.at(forget_query) == 0)
      histogram_.erase(forget_query);

    delete forget;

    count_--;
  }

  query_order_->PushBack(new DoublyLinkedList<std::string>::Node(query));
  count_++;
  histogram_[query]++;

  return histogram_.at(query);
}

std::uint32_t WindowLfuHistogram::Estimate(const std::string &query) const {
  if (histogram_.find(query) != histogram_.end())
    return histogram_.at(query);

  return 0;
}

std::string WindowLfuHistogram::ToString() const {
  return "";
}
