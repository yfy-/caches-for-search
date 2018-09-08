// Copyright 2017 folly
#include <string>
#include "cache/lfu_cache.h"

/* TODO: Refactor all of this and other caches
 * when you have time (too much code repetition)..
 * Also, you can swap to two dimensional cache implementations
 * which are O(1) in any operation.
 */
LfuCache::LfuCache(std::uint32_t s) : Cache(s) {
  cache_ = new NodeDoublyLinkedList();
}

LfuCache::~LfuCache() {
  delete cache_;
}

bool LfuCache::IsExist(const std::string &query) {
  if (freq_hist_ == nullptr)
      return InMemoryLfu(query);

  bool result = false;

  std::uint32_t estimated_freq = freq_hist_->Add(query);

  NodeDoublyLinkedList::Node* current = cache_table_[query];

  if (current != nullptr)
    result = true;

  if (result) {
    current->score = estimated_freq;
    PutBeforeMoreFrequent(current);
  } else {
    if (count_ < size_) {
      current = new NodeDoublyLinkedList::Node(query, estimated_freq);
      cache_->PushFront(current);
      cache_table_[query] = current;
      PutBeforeMoreFrequent(current);
      count_++;
    } else {
      NodeDoublyLinkedList::Node* victim = cache_->head_->next;

      if (estimated_freq >= freq_hist_->Estimate(victim->data)) {
        cache_table_.erase(victim->data);
        victim->data = query;
        victim->score = estimated_freq;
        cache_table_[query] = victim;
        PutBeforeMoreFrequent(victim);
      }
    }
  }

  return result;
}

bool LfuCache::InMemoryLfu(const std::string& query) {
  bool result = false;

  NodeDoublyLinkedList::Node* current = cache_table_[query];

  if (current != nullptr) {
    result = true;
    current->score += 1;
    PutBeforeMoreFrequent(current);
  } else {
    if (count_ < size_) {
      current = new NodeDoublyLinkedList::Node(query, 1);
      cache_->PushFront(current);
      cache_table_[query] = current;
      PutBeforeMoreFrequent(current);
      count_++;
    } else {
      NodeDoublyLinkedList::Node* victim = cache_->head_->next;
      cache_table_.erase(victim->data);
      victim->data = query;
      victim->score = 1;
      cache_table_[query] = victim;
      PutBeforeMoreFrequent(victim);
    }
  }

  return result;
}

void LfuCache::PutBeforeMoreFrequent(NodeDoublyLinkedList::Node *node) {
  NodeDoublyLinkedList::Node* more_freq = nullptr;
  NodeDoublyLinkedList::Node* curr = node;

  while ((curr->next != cache_->tail_) && (curr->next != nullptr)) {
    if (node->score < curr->next->score) {
      more_freq = curr->next;
      cache_->MoveBefore(node, more_freq);
      break;
    }
    curr = curr->next;
  }

  if(more_freq == nullptr)
    cache_->MoveToBack(node);
}

std::string LfuCache::ToString() const {
 return cache_->ToString();
}
