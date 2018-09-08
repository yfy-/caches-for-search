// Created by yfy on 28/03/2018

#ifndef CENG778PROJECT_POLICY_EVICTION_LRUEVICTION_H
#define CENG778PROJECT_POLICY_EVICTION_LRUEVICTION_H

#include <unordered_map>
#include "doubly_linked_list.h"
#include "policy/eviction/eviction_policy.h"

class LruEviction : public EvictionPolicy {
 public:
  LruEviction(std::uint32_t s) : EvictionPolicy(s) {}

  bool IsExist(const std::string&) const override;
  void Update(const std::string&, std::uint32_t = 1) override;
  bool IsEmpty() const override;
  std::string Victim() const override;
  std::string ReplaceVictim(const std::string&, std::uint32_t = 1) override;
  void Add(const std::string&, std::uint32_t = 1) override;
  std::string CacheToString() const override;

  virtual ~LruEviction();

 private:
  typedef DoublyLinkedList<std::string>::Node LruNode;

  DoublyLinkedList<std::string> cache_;
  std::unordered_map<std::string, LruNode*> cache_table_;

  LruNode* VictimNode() const;
};

#endif // CENG778PROJECT_POLICY_EVICTION_LRUEVICTION_H
