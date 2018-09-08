//
// Created by yfy on 10/22/17.
//

#include <iostream>
#include "cache/greedy_dual_size_k_cache.h"

constexpr float GreedyDualSizeKCache::estimated_freq_[20];

GreedyDualSizeKCache::GreedyDualSizeKCache(std::uint32_t s, std::uint32_t k) :
  Cache(s), k_{k}, offset_{0.0} {

    cache_ = new NodeDoublyLinkedList();
}

GreedyDualSizeKCache::~GreedyDualSizeKCache() {
  delete cache_;
}

bool GreedyDualSizeKCache::IsExist(const std::string& query) {
  if (freq_hist_ == nullptr)
    return InMemoryGdsfk(query);

  bool result = false;

  std::uint32_t estimated_freq = freq_hist_->Add(query);
  NodeDoublyLinkedList::Node* current = cache_table_[query];

  if (current != nullptr)
    result = true;

  if (result) {
    current->score = HValue(estimated_freq);

    PutBeforeHigherHValue(current);
  } else {
    if (count_ < size_) {
      current = new NodeDoublyLinkedList::Node(query, HValue(estimated_freq));
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
        victim->score = HValue(estimated_freq);
        cache_table_[query] = victim;
        PutBeforeHigherHValue(victim);
      }
    }
  }

  return result;
}

bool GreedyDualSizeKCache::InMemoryGdsfk(const std::string& query) {
  bool result = false;
  NodeDoublyLinkedList::Node* current = cache_table_[query];

  if (current != nullptr) {
    result = true;
    current->freq += 1;
    current->score = HValue(current->freq);
    PutBeforeHigherHValue(current);
  } else {
    if (count_ < size_) {
      current = new NodeDoublyLinkedList::Node(query, HValue(1));
      current->freq = 1;
      cache_->PushFront(current);
      cache_table_[query] = current;
      PutBeforeHigherHValue(current);
      count_++;
    } else {
      NodeDoublyLinkedList::Node* victim = cache_->head_->next;
      offset_ = victim->score;
      cache_table_.erase(victim->data);
      victim->data = query;
      victim->score = HValue(1);
      victim->freq = 1;
      cache_table_[query] = victim;
      PutBeforeHigherHValue(victim);
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
  return "Aging Factor: " + std::to_string(offset_) + "\n" + cache_->ToString();
}
