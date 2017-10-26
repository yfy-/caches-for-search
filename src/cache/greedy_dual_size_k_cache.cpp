//
// Created by yfy on 10/22/17.
//

#include "cache/greedy_dual_size_k_cache.h"

GreedyDualSizeKCache::GreedyDualSizeKCache(std::uint32_t s,
                                           FrequencyHistogram* fh) : Cache(s) {
  cache_ = new NodeDoublyLinkedList();
  freq_hist_ = fh;
  offset_ = 0;
}

GreedyDualSizeKCache::~GreedyDualSizeKCache() {
  delete cache_;
}

bool GreedyDualSizeKCache::IsExist(const std::string& query) {
  bool result = false;

  std::uint32_t estimated_freq = freq_hist_->Add(query);
  NodeDoublyLinkedList::Node* current = cache_table_[query];

  if (current != nullptr)
    result = true;

  if (result) {
    current->score = offset_ + current->score + 1;
    PutBeforeHigherHValue(current);
  } else {
    if (count_ < size_) {
      current = new NodeDoublyLinkedList::Node(query, estimated_freq + offset_);
      cache_->PushFront(current);
      cache_table_[query] = current;
      PutBeforeHigherHValue(current);
      count_++;
    } else {
      NodeDoublyLinkedList::Node* victim = cache_->head_->next;

      if (estimated_freq >= freq_hist_->Estimate(victim->data)) {
        offset_ = victim->score;
        cache_table_.erase(victim->data);
        victim->data = query;
        victim->score = estimated_freq + offset_;
        cache_table_[query] = victim;
        PutBeforeHigherHValue(victim);
      }
    }
  }

  return result;
}

void GreedyDualSizeKCache::PutBeforeHigherHValue(NodeDoublyLinkedList::Node* node) {
  NodeDoublyLinkedList::Node* more_hvalue = nullptr;
  NodeDoublyLinkedList::Node* curr = node;

  while ((curr->next != cache_->tail_) && (curr->next != nullptr)) {
    if (node->score < curr->next->score) {
      more_hvalue = curr->next;
      cache_->MoveBefore(node, more_hvalue);
      break;
    }
    curr = curr->next;
  }

  if (more_hvalue == nullptr)
    cache_->MoveToBack(node);
}

std::string GreedyDualSizeKCache::ToString() const {
  return cache_->ToString();
}
