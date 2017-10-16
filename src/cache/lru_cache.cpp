//
// Created by yfy on 10/15/17.
//

#include <string>
#include "cache/lru_cache.h"

LruCache::LruCache(uint32_t s) : Cache(s) {
  cache_ = new NodeDoublyLinkedList();
}

LruCache::~LruCache() {
  delete cache_;
}

bool LruCache::IsExist(const std::string &query) {
  if (freq_hist_ != nullptr)
    return IsExistWithFreqHist(query);

  bool result = false;
  NodeDoublyLinkedList::Node* current = cache_table_[query];

  if (current != nullptr) {
    result = true;
    cache_->MoveToBack(current);
  } else {
    if (count_ < size_) {
      current = new NodeDoublyLinkedList::Node(query, 0);
      cache_->PushBack(current);
      cache_table_[query]= current;
      count_++;
    } else {
      NodeDoublyLinkedList::Node* victim = cache_->head_->next;
      cache_table_.erase(victim->data);
      victim->data = query;
      cache_table_[query] = victim;
      cache_->MoveToBack(victim);
    }
  }

  return result;
}
std::string LruCache::ToString() const {
  return cache_->ToString();
}

bool LruCache::IsExistWithFreqHist(const std::string &query) {
  bool result = false;

  std::uint32_t estimated_freq = freq_hist_->Add(query);

  NodeDoublyLinkedList::Node* current = cache_table_[query];

  if (current != nullptr) {
    result = true;
    cache_->MoveToBack(current);
  } else {
    if (count_ < size_) {
      current = new NodeDoublyLinkedList::Node(query, 0);
      cache_->PushBack(current);
      cache_table_[query] = current;
      count_++;
    } else {
      NodeDoublyLinkedList::Node* victim = cache_->head_->next;

      if (estimated_freq >= freq_hist_->Estimate(victim->data)) {
        cache_table_.erase(victim->data);
        victim->data = query;
        cache_table_[query] = victim;
        cache_->MoveToBack(victim);
      }
    }
  }

  return result;
}

