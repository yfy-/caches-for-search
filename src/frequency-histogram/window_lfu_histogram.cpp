//
// Created by yfy on 10/28/17.
//

#include "frequency-histogram/window_lfu_histogram.h"

WindowLfuHistogram::WindowLfuHistogram(std::uint32_t w) : window_{w}, count_{0}
{
  query_order_ = new NodeDoublyLinkedList();
}
WindowLfuHistogram::~WindowLfuHistogram() {
  delete query_order_;
}

uint32_t WindowLfuHistogram::Add(const std::string &query) {

  if (count_ == window_) {
    NodeDoublyLinkedList::Node* forget = query_order_->head_->next;

    query_order_->Disconnect(forget);
    histogram_[forget->data]--;

    if (histogram_.at(forget->data) == 0)
      histogram_.erase(forget->data);

    delete forget;

    count_--;
  }

  query_order_->PushBack(new NodeDoublyLinkedList::Node(query, 0));
  count_++;
  histogram_[query]++;

  return histogram_.at(query);
}

uint32_t WindowLfuHistogram::Estimate(const std::string &query) const {
  if (histogram_.find(query) != histogram_.end())
    return histogram_.at(query);

  return 0;
}

std::string WindowLfuHistogram::ToString() const {
  std::string repr = "Order\n" + query_order_->ToString() + "\nHistogram\n";

  for (auto x : histogram_) {
    repr += x.first + ":" + std::to_string(x.second) + ", ";
  }
  return repr.substr(0, repr.size() - 1);
}
