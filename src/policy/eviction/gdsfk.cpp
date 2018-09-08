// Created by yfy on 13/05/2018

#include <cstdint>
#include <iostream>
#include "policy/eviction/gdsfk.h"

Gdsfk::Gdsfk(std::uint32_t s, std::uint8_t k) : EvictionPolicy(s),
                                                 kFreqExp{k},
                                                aging_factor_{0},
                                                freq_hist_{nullptr},
                                                max_hval_{0} {}

Gdsfk::~Gdsfk() {
  for (auto& kv : cache_)
    delete kv.second;
}

bool Gdsfk::IsExist(const std::string& query) const {
  return cache_table_.find(query) != cache_table_.end();
}

bool Gdsfk::IsEmpty() const {
  return count_ < size_;
}

void Gdsfk::SetFrequencyHistogram(FrequencyHistogram* fh) {
  freq_hist_ = fh;
}

void Gdsfk::Add(const std::string& query, std::uint32_t adm_freq) {
  std::uint32_t freq =
      freq_hist_ == nullptr ? adm_freq : freq_hist_->Add(query);

  std::uint64_t hval = HValue(freq);
  auto qnode = new GdsfkQueryNode(query);
  Add(qnode, hval);
  count_++;
}

void Gdsfk::Update(const std::string& query, std::uint32_t adm_freq) {
  std::uint32_t freq =
      freq_hist_ == nullptr ? adm_freq : freq_hist_->Add(query);

  std::uint64_t new_hval = HValue(freq);
  std::pair<std::uint64_t, GdsfkQueryNode*> hval_q = cache_table_.at(query);
  auto prev_hval = hval_q.first;
  auto qnode = hval_q.second;

  cache_[prev_hval]->Disconnect(qnode);
  Add(qnode, new_hval);
}

std::string Gdsfk::ReplaceVictim(const std::string& query,
                                 std::uint32_t adm_freq) {

  std::uint32_t freq =
      freq_hist_ == nullptr ? adm_freq : freq_hist_->Add(query);


  std::uint64_t victim_hval = VictimHVal();
  DoublyLinkedList<std::string>* victim_lru = cache_.at(victim_hval);
  GdsfkQueryNode* victim = victim_lru->head_->next;
  std::string qevict = victim->content;

  victim->content = query;
  aging_factor_ = victim_hval;
  std::uint64_t new_hval = HValue(freq);
  victim_lru->Disconnect(victim);
  cache_table_.erase(qevict);
  Add(victim, new_hval);

  return qevict;
}

std::string Gdsfk::Victim() const {
  return cache_.at(VictimHVal())->head_->next->content;
}

std::string Gdsfk::CacheToString() const {
  std::string res = "Aging Factor = " + std::to_string(aging_factor_) + "\n\n";

  for(auto& kv : cache_) {
    DoublyLinkedList<std::string>* lru = kv.second;
    if (!lru->IsEmpty()) {
      res += std::to_string(kv.first) + ": <head> -> ";
      GdsfkQueryNode* q_crr = lru->head_->next;

      while (q_crr != lru->tail_) {
        res += q_crr->content + " -> ";
        q_crr = q_crr->next;
      }

      res += "<tail>\n";
    }
  }

  return res;
}

void Gdsfk::Add(GdsfkQueryNode* node, std::uint64_t hval) {
  bool ex = cache_.find(hval) != cache_.end();

  if (!ex)
    cache_[hval] = new DoublyLinkedList<std::string>();

  cache_[hval]->PushBack(node);
  cache_table_[node->content] = std::make_pair(hval, node);

  if (hval > max_hval_)
    max_hval_ = hval;
}

std::uint64_t Gdsfk::VictimHVal() const {
  for (std::uint64_t i = aging_factor_; i <= max_hval_; ++i) {
    if (cache_.find(i) != cache_.end()) {
      DoublyLinkedList<std::string>* lru = cache_.at(i);
      if (!lru->IsEmpty())
        return i;
    }
  }

  return 0;
}
