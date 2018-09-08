// Created by yfy on 12/05/2018

#include <limits>
#include "policy/eviction/inmemory_gdsfk.h"

InmemoryGdsfk::InmemoryGdsfk(std::uint32_t s, std::uint8_t k) :
    EvictionPolicy(s), kFreqExp{k}, aging_factor_{0}, max_hval_{0} {}

InmemoryGdsfk::~InmemoryGdsfk() {
  for (auto& kv : cache_) {
    DoublyLinkedList<QueryNode*>* lru = kv.second;
    GdsfkQueryNode* crr = lru->head_->next;

    while (crr != lru->tail_) {
      delete crr->content;
      crr = crr->next;
    }

    delete lru;
  }
}

bool InmemoryGdsfk::IsExist(const std::string& query) const {
  return cache_table_.find(query) != cache_table_.end();
}

bool InmemoryGdsfk::IsEmpty() const {
  return count_ < size_;
}

void InmemoryGdsfk::Add(const std::string& query, std::uint32_t adm_freq) {
  std::uint32_t freq = 1;
  auto qnode = new GdsfkQueryNode(new QueryNode{ freq, query });
  std::uint64_t hval = HValue(freq);

  Add(qnode, hval, query);
  count_++;
}

void InmemoryGdsfk::Update(const std::string& query, std::uint32_t adm_freq) {
  std::pair<std::uint64_t, GdsfkQueryNode*> hval_node = cache_table_.at(query);
  DoublyLinkedList<QueryNode*>* lru_list = cache_[hval_node.first];
  GdsfkQueryNode* node = hval_node.second;

  lru_list->Disconnect(node);
  node->content->freq++;

  std::uint64_t new_hval = HValue(node->content->freq);

  Add(node, new_hval, query);
}

void InmemoryGdsfk::Add(GdsfkQueryNode* node, std::uint64_t hval,
                        const std::string& query) {
  bool hv_ex = cache_.find(hval) != cache_.end();
  auto lru_to_add = hv_ex ? cache_[hval] : new DoublyLinkedList<QueryNode*>();

  lru_to_add->PushBack(node);

  if (!hv_ex)
    cache_[hval] = lru_to_add;

  if (hval > max_hval_)
    max_hval_ = hval;

  cache_table_[query] = std::make_pair(hval, node);
}

std::string InmemoryGdsfk::ReplaceVictim(const std::string& query,
                                         std::uint32_t adm_freq) {
  std::uint64_t victim_hval = VictimHVal();
  DoublyLinkedList<QueryNode*>* victim_lru = cache_[victim_hval];
  GdsfkQueryNode* victim_node = victim_lru->head_->next;
  QueryNode* victim_node_content = victim_node->content;
  std::string evicted_query = victim_node_content->query;

  cache_table_.erase(evicted_query);
  victim_node_content->query = query;
  std::uint32_t freq = 1;
  victim_node_content->freq = freq;
  victim_lru->Disconnect(victim_node);
  aging_factor_ = victim_hval;
  Add(victim_node, HValue(freq), query);

  return evicted_query;
}

std::uint64_t InmemoryGdsfk::VictimHVal() const {
  for (std::uint64_t i = 1; i <= max_hval_; i++) {
    if (cache_.find(i) != cache_.end()) {
      DoublyLinkedList<QueryNode*>* lru = cache_.at(i);
      if (!lru->IsEmpty())
        return i;
    }
  }

  return 0;
}

std::string InmemoryGdsfk::Victim() const {
  return cache_.at(VictimHVal())->head_->next->content->query;
}

std::string InmemoryGdsfk::CacheToString() const {
  std::string res = "Aging Factor = " + std::to_string(aging_factor_) + "\n\n";

  for (auto& kv : cache_) {
    DoublyLinkedList<QueryNode*>* lru = kv.second;
    if (!lru->IsEmpty()) {
      res += std::to_string(kv.first) + ": <head> -> ";
      GdsfkQueryNode* crr = lru->head_->next;

      while (crr != lru->tail_) {
        QueryNode* crr_cont = crr->content;
        res += crr_cont->query + ";" + std::to_string(crr_cont->freq) + " -> ";
        crr = crr->next;
      }

      res += "<tail>\n";
    }
  }

  return res;
}
