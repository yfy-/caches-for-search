// Created by yfy on 31/03/2018

#ifndef CENG778PROJECT_POLICY_EVICTION_INMEMORYLFUEVICTION_H
#define CENG778PROJECT_POLICY_EVICTION_INMEMORYLFUEVICTION_H

#include <unordered_map>
#include <utility>
#include "doubly_linked_list.h"
#include "policy/eviction/eviction_policy.h"

class InMemoryLfuEviction : public EvictionPolicy {

 public:
  InMemoryLfuEviction(std::uint32_t s) : EvictionPolicy(s) {}
  virtual ~InMemoryLfuEviction();
  void Update(const std::string&, std::uint32_t = 1) override;
  bool IsExist(const std::string&) const override;
  bool IsEmpty() const override;
  std::string Victim() const override;
  std::string ReplaceVictim(const std::string&, std::uint32_t = 1) override;
  void Add(const std::string&, std::uint32_t = 1) override;
  std::string CacheToString() const override;

 private:

  struct FreqNode {
    std::uint32_t freq;
    DoublyLinkedList<std::string>* lru_list;
    FreqNode(std::uint32_t f) : freq{f} {
      lru_list = new DoublyLinkedList<std::string>();
    }

    ~FreqNode() {
      delete lru_list;
    }
  };

  typedef DoublyLinkedList<FreqNode*>::Node LfuFreqNode;
  typedef DoublyLinkedList<std::string>::Node LfuQueryNode;
  DoublyLinkedList<FreqNode*> cache_;
  std::unordered_map<std::string, std::pair<LfuFreqNode*,
                                            LfuQueryNode*>> cache_table_;

  LfuQueryNode* VictimNode() const;
  LfuFreqNode* VictimFreqNode() const;
  LfuFreqNode* CreateFirstFreqNode();
};

#endif // CENG778PROJECT_POLICY_EVICTION_INMEMORYLFUEVICTION_H
