// Copyright 2017 folly
#include <string>
#include "cache/lfu_cache.h"

LfuCache::LfuCache(std::uint64_t s, FrequencyHistogram* fh) : Cache(s) {
  head_ = new Node("");
  freq_hist_ = fh;
}

LfuCache::~LfuCache() {
  Node* current = head_;

  while (current != nullptr) {
    Node* next = current->next;
    delete current;
    current = next;
  }
}

bool LfuCache::IsExist(const std::string &query) {
  bool result = false;

  std::uint64_t new_freq = freq_hist_->Add(query);

  Node* current = cache_table_[query];

  if (current != nullptr)
    result = true;

  if (result) {
    FindPlace(current, new_freq);
  } else {
    if (count_ < size_) {
      Node* node = new Node(query);
      Node* next = head_->next;

      head_->next = node;
      node->next = next;
      cache_table_[query] = node;
      FindPlace(node, new_freq);
      count_++;
    } else {
      Node* victim = head_->next;

      if (new_freq >= freq_hist_->Estimate(victim->data)) {
        cache_table_.erase(victim->data);
        victim->data = query;
        cache_table_[query] = victim;
        FindPlace(victim, new_freq);
      }
    }
  }

  return result;
}

void LfuCache::FindPlace(Node* node, std::uint64_t freq) {
  while (node->next != nullptr &&
         freq >= freq_hist_->Estimate(node->next->data)) {
    Node* next = node->next;

    std::string temp_data = next->data;

    next->data = node->data;
    node->data = temp_data;
    cache_table_[node->data] = node;
    cache_table_[next->data] = next;
    node = node->next;
  }
}

std::string LfuCache::ToString() {
  std::string result = "";

  Node* current = head_->next;

  while (current != nullptr) {
    result += current->data + ":" +
        std::to_string(freq_hist_->Estimate(current->data)) +" -> ";
    current = current->next;
  }

  return result;
}
