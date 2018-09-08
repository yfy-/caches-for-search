// Created by yfy on 01/04/2018

#ifndef CENG778PROJECT_POLICY_EVICTION_LFUEVICTION_H
#define CENG778PROJECT_POLICY_EVICTION_LFUEVICTION_H

#include <unordered_map>
#include "doubly_linked_list.h"
#include "policy/eviction/eviction_policy.h"
#include "frequency-histogram/pure_lfu_histogram.h"

class LfuEviction : public EvictionPolicy {

 public:

  LfuEviction(std::uint32_t, std::uint32_t);
  virtual ~LfuEviction();
  void Update(const std::string&, std::uint32_t = 1) override;
  bool IsExist(const std::string&) const override;
  bool IsEmpty() const override;
  std::string Victim() const override;
  std::string ReplaceVictim(const std::string&, std::uint32_t = 1) override;
  void Add(const std::string&, std::uint32_t = 1) override;
  std::string CacheToString() const override;

  inline void SetFrequencyHistogram(FrequencyHistogram* fh) {
    freq_hist_ = fh;
  }

 private:

  typedef DoublyLinkedList<std::string>::Node LfuNode;
  DoublyLinkedList<std::string>** cache_;
  std::unordered_map<std::string,
                     std::pair<std::uint32_t, LfuNode*>> cache_table_;
  std::uint32_t max_freq_;
  FrequencyHistogram* freq_hist_;

  LfuNode* VictimNode() const;
  std::uint32_t VictimFreq() const;
};

#endif // CENG778PROJECT_POLICY_EVICTION_LFUEVICTION_H
