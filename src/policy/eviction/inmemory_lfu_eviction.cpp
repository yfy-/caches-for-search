// Created by yfy on 31/03/2018

#include "policy/eviction/inmemory_lfu_eviction.h"
#include "policy/eviction/eviction_policy.h"

bool InMemoryLfuEviction::IsExist(const std::string& query) const {
  return cache_table_.find(query) != cache_table_.end();
}

void InMemoryLfuEviction::Update(const std::string& query, std::uint32_t freq) {
  std::pair<LfuFreqNode*, LfuQueryNode*> crr = cache_table_.at(query);
  LfuFreqNode* freqn = crr.first;
  LfuQueryNode* queryn = crr.second;

  DoublyLinkedList<std::string>* crr_lru = freqn->content->lru_list;
  crr_lru->Disconnect(queryn);
  std::uint32_t new_freq = freqn->content->freq + 1;

  if (freqn->next == cache_.tail_ || new_freq != freqn->next->content->freq)
    cache_.PushAfter(new LfuFreqNode(new FreqNode(new_freq)), freqn);

  LfuFreqNode* next_freqn = freqn->next;
  // next_freqn->content->lru_list->PushBack(queryn);
  next_freqn->content->lru_list->PushFront(queryn);
  cache_table_[query] = std::make_pair(next_freqn, queryn);
}

bool InMemoryLfuEviction::IsEmpty() const {
  return count_ < size_;
}

// Causes nullptr dereference if called on empty cache
std::string InMemoryLfuEviction::Victim() const {
  return VictimNode()->content;
}

InMemoryLfuEviction::LfuFreqNode* InMemoryLfuEviction::VictimFreqNode() const {
  LfuFreqNode* crr = cache_.head_->next;

  while (crr != cache_.tail_) {
    if (!crr->content->lru_list->IsEmpty())
      return crr;
    crr = crr->next;
  }

  return nullptr;
}

// Causes nullptr dereference if called on empty cache
InMemoryLfuEviction::LfuQueryNode* InMemoryLfuEviction::VictimNode() const {
  return VictimFreqNode()->content->lru_list->head_->next;
}

InMemoryLfuEviction::LfuFreqNode* InMemoryLfuEviction::CreateFirstFreqNode() {
  if (count_ == 0 || cache_.head_->next->content->freq != 1)
    cache_.PushFront(new LfuFreqNode(new FreqNode(1)));

  return cache_.head_->next;
}

void InMemoryLfuEviction::Add(const std::string& query, std::uint32_t freq) {
  LfuFreqNode* lfn = CreateFirstFreqNode();
  LfuQueryNode* lqn = new LfuQueryNode(query);
  // lfn->content->lru_list->PushBack(lqn);
  lfn->content->lru_list->PushFront(lqn);
  cache_table_[query] = std::make_pair(lfn, lqn);
  count_++;
}

std::string InMemoryLfuEviction::ReplaceVictim(const std::string& query,
                                               std::uint32_t freq) {
  LfuQueryNode* victim = VictimNode();
  LfuFreqNode* victim_freq = VictimFreqNode();
  std::string v_content = victim->content;

  cache_table_.erase(v_content);

  DoublyLinkedList<std::string>* victim_lru = victim_freq->content->lru_list;
  victim_lru->Disconnect(victim);
  victim->content = query;

  LfuFreqNode* first_freq = CreateFirstFreqNode();
  // first_freq->content->lru_list->PushBack(victim);
  first_freq->content->lru_list->PushFront(victim);
  cache_table_[query] = std::make_pair(first_freq, victim);

  return v_content;
}

std::string InMemoryLfuEviction::CacheToString() const {
  LfuFreqNode* freq_crr = VictimFreqNode();
  std::string resp = "";

  while (freq_crr != cache_.tail_) {
    DoublyLinkedList<std::string>* q_lru_list = freq_crr->content->lru_list;

    if (!q_lru_list->IsEmpty()) {
      resp += std::to_string(freq_crr->content->freq) + ": <head> -> ";

      LfuQueryNode* query_crr = q_lru_list->head_->next;

      while (query_crr != q_lru_list->tail_) {
        resp += query_crr->content + " -> ";
        query_crr = query_crr->next;
      }

      resp += "<tail>\n";
    }

    freq_crr = freq_crr->next;
  }

  return resp;
}

InMemoryLfuEviction::~InMemoryLfuEviction() {
  LfuFreqNode* crr = VictimFreqNode();

  while (crr != cache_.tail_) {
    delete crr->content;
    crr = crr->next;
  }
}
