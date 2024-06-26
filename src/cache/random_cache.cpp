
//
// Created by yfy on 10/9/17.
//

#include <cstdlib>
#include "cache/random_cache.h"

RandomCache::RandomCache(uint32_t s) : Cache(s) {
  cache_ = new NodeDoublyLinkedList::Node*[size_];
}

bool RandomCache::IsExist(const std::string &query) {
  if (freq_hist_ != nullptr) {
    return IsExistWithFreqHist(query);
  }

  if (cache_table_[query] != nullptr) {
    return true;
  }

  if (count_ < size_) {
    AddNewEntry(query);
  } else {
    NodeDoublyLinkedList::Node* victim = cache_[rand() % count_];
    ReplaceVictim(query, victim);
  }

  return false;
}

bool RandomCache::IsExistWithFreqHist(const std::string &query) {
  std::uint32_t new_freq = freq_hist_->Add(query);

  if (cache_table_[query] != nullptr) {
    return true;
  }

  if (count_ < size_) {
    AddNewEntry(query);
  } else {
    NodeDoublyLinkedList::Node* victim = cache_[rand() % count_];

    if(new_freq >= freq_hist_->Estimate(victim->data)) {
      ReplaceVictim(query, victim);
    }
  }

  return false;
}
void RandomCache::AddNewEntry(const std::string &query) {
  NodeDoublyLinkedList::Node* node = new NodeDoublyLinkedList::Node(query, 0);
  cache_[count_] = node;
  cache_table_[query] = node;
  count_++;
}

void RandomCache::ReplaceVictim(const std::string &query, NodeDoublyLinkedList::Node* victim) {
  cache_table_.erase(victim->data);
  victim->data = query;
  cache_table_[query] = victim;
}

RandomCache::~RandomCache() {
  for (int i = 0; i < count_; ++i) {
    delete cache_[i];
  }

  delete cache_;
}
