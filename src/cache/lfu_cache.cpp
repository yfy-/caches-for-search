// Copyright 2017 folly
#include <string>
#include "cache/lfu_cache.h"

LfuCache::LfuCache(std::uint32_t s, FrequencyHistogram* fh) : Cache(s) {
  cache_ = new NodeDoublyLinkedList();
  freq_hist_ = fh;
}

LfuCache::~LfuCache() {
  delete cache_;
}

bool LfuCache::IsExist(const std::string &query) {
  bool result = false;

  std::uint32_t new_freq = freq_hist_->Add(query);

  NodeDoublyLinkedList::Node* current = cache_table_[query];

  if (current != nullptr)
    result = true;

  if (result) {
    PutBeforeMoreFrequent(current, new_freq);
  } else {
    if (count_ < size_) {
      current = new NodeDoublyLinkedList::Node(query);
      cache_->PushFront(current);
      cache_table_[query] = current;
      PutBeforeMoreFrequent(current, new_freq);
      count_++;
    } else {
      NodeDoublyLinkedList::Node* victim = cache_->head_->next;

      if (new_freq >= freq_hist_->Estimate(victim->data)) {
        cache_table_.erase(victim->data);
        victim->data = query;
        cache_table_[query] = victim;
        PutBeforeMoreFrequent(victim, new_freq);
      }
    }
  }

  return result;
}

void LfuCache::PutBeforeMoreFrequent(NodeDoublyLinkedList::Node *node, std::uint32_t freq) {
  NodeDoublyLinkedList::Node* more_freq = nullptr;
  NodeDoublyLinkedList::Node* curr = node;
  while ((curr->next != cache_->tail_) && (curr->next != nullptr)) {
    if (freq < freq_hist_->Estimate(curr->next->data)) {
      more_freq = curr->next;
      cache_->MoveBefore(node, more_freq);
      break;
    }
    curr = curr->next;
  }

  if(more_freq == nullptr)
    cache_->MoveToBack(node);
}

/*std::string LfuCache::ToString() {
  std::string result;

  NodeDoublyLinkedList::Node* current = head_->next;

  while (current != nullptr) {
    result += current->data + ":" +
        std::to_string(freq_hist_->Estimate(current->data)) +" -> ";
    current = current->next;
  }

  return result;
}*/
