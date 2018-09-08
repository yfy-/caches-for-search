// Created by yfy on 28/03/2018

#include "policy/eviction/lru_eviction.h"

bool LruEviction::IsExist(const std::string& query) const {
  return cache_table_.find(query) != cache_table_.end();
}

void LruEviction::Update(const std::string& query, std::uint32_t freq) {
  LruNode* crr = cache_table_[query];
  cache_.MoveToBack(crr);
}

bool LruEviction::IsEmpty() const {
  return count_ < size_;
}

std::string LruEviction::Victim() const {
  return VictimNode()->content;
}

std::string LruEviction::ReplaceVictim(const std::string& query,
                                       std::uint32_t freq) {
  LruNode* victim = VictimNode();
  std::string evicted_content = victim->content;

  cache_table_.erase(evicted_content);
  victim->content = query;
  cache_table_[query] = victim;
  cache_.MoveToBack(victim);

  return evicted_content;
}

void LruEviction::Add(const std::string& query, std::uint32_t freq) {
  LruNode* crr = new LruNode(query);
  cache_.PushBack(crr);
  cache_table_[query] = crr;
  count_++;
}

LruEviction::LruNode* LruEviction::VictimNode() const {
  return cache_.head_->next;
}

std::string LruEviction::CacheToString() const {
  std::string rep = "<head> -> ";
  LruNode* crr = cache_.head_->next;

  while (crr != cache_.tail_) {
    rep += crr->content + " -> ";
    crr = crr->next;
  }

  rep += "<tail>";

  return rep;
}

LruEviction::~LruEviction() {}
