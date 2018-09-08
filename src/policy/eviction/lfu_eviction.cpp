// Created by yfy on 01/04/2018

#include "policy/eviction/lfu_eviction.h"

LfuEviction::LfuEviction(std::uint32_t s, std::uint32_t mf) :
    EvictionPolicy(s), max_freq_{mf} {
  cache_ = new DoublyLinkedList<std::string>*[max_freq_ + 1]();
  freq_hist_ = nullptr;
}

void LfuEviction::Update(const std::string& query, std::uint32_t adm_freq) {
  std::uint32_t estimate = 0;

  /* If freq returned from Add is bigger than max freq, the index will be out of
     range. The check isn't here intentionally, because max freq should always
     be set appropriately.
  */

  if (freq_hist_ != nullptr)
    estimate = freq_hist_->Add(query);

  std::pair<std::uint32_t, LfuNode*> freq_list = cache_table_[query];
  std::uint32_t freq = freq_list.first;
  LfuNode* ln = freq_list.second;

  cache_[freq]->Disconnect(ln);

  std::uint32_t new_freq = estimate > 0 ? estimate : adm_freq;

  if (cache_[new_freq] == nullptr)
    cache_[new_freq] = new DoublyLinkedList<std::string>();

  // cache_[new_freq]->PushBack(ln);
  cache_[new_freq]->PushFront(ln);
  cache_table_[query] = std::make_pair(adm_freq, ln);
}

bool LfuEviction::IsExist(const std::string& query) const {
  return cache_table_.find(query) != cache_table_.end();
}

bool LfuEviction::IsEmpty() const {
  return count_ < size_;
}

std::string LfuEviction::Victim() const {
  LfuNode* v = VictimNode();
  return v == nullptr ? "" : v->content;
}

void LfuEviction::Add(const std::string& query, std::uint32_t adm_freq) {
  std::uint32_t new_freq = freq_hist_ == nullptr ? adm_freq :
                           freq_hist_->Add(query);

  if (cache_[new_freq] == nullptr)
    cache_[new_freq] = new DoublyLinkedList<std::string>();

  LfuNode* nq = new LfuNode(query);
  // cache_[new_freq]->PushBack(nq);
  cache_[new_freq]->PushFront(nq);
  cache_table_[query] = std::make_pair(new_freq, nq);
  count_++;
}

std::string LfuEviction::ReplaceVictim(const std::string& query,
                                       std::uint32_t adm_freq) {
  LfuNode* victim = VictimNode();
  std::uint32_t victim_freq = VictimFreq();

  cache_[victim_freq]->Disconnect(victim);
  std::string evicted_content = victim->content;
  cache_table_.erase(evicted_content);

  victim->content = query;

  std::uint32_t new_freq = freq_hist_ == nullptr ? adm_freq :
                           freq_hist_->Add(query);

  if (cache_[new_freq] == nullptr)
    cache_[new_freq] = new DoublyLinkedList<std::string>();

  // cache_[new_freq]->PushBack(victim);
  cache_[new_freq]->PushFront(victim);
  cache_table_[query] = std::make_pair(new_freq, victim);
  return evicted_content;
}

std::string LfuEviction::CacheToString() const {
  std::string rep = "";

  for (std::uint32_t i = 0; i < max_freq_; ++i) {
    DoublyLinkedList<std::string>* crr_list = cache_[i];
    if (crr_list != nullptr && !crr_list->IsEmpty()) {
      rep += std::to_string(i) + ": <head> -> ";
      LfuNode* crr = crr_list->head_->next;

      while (crr != crr_list->tail_) {
        rep += crr->content + " -> ";
        crr = crr->next;
      }

      rep += "<tail>\n";
    }
  }

  return rep;
}

LfuEviction::~LfuEviction() {
  for (std::uint32_t i = 1; i <= max_freq_; ++i) {
    if (cache_[i] != nullptr)
      delete cache_[i];
  }
  delete cache_;
}

LfuEviction::LfuNode* LfuEviction::VictimNode() const {
  return cache_[VictimFreq()]->head_->next;
}

std::uint32_t LfuEviction::VictimFreq() const {
  for (std::uint32_t i = 1; i < max_freq_; ++i) {
    if (cache_[i] != nullptr && !cache_[i]->IsEmpty())
      return i;
  }

  return 0;
}
