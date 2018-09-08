// Created by yfy on 13/05/2018

#ifndef CENG778PROJECT_POLICY_EVICTION_GDSFK_H
#define CENG778PROJECT_POLICY_EVICTION_GDSFK_H

#include <cmath>
#include <cstdint>
#include <utility>
#include <unordered_map>
#include <string>
#include "doubly_linked_list.h"
#include "policy/eviction/eviction_policy.h"
#include "frequency-histogram/frequency_histogram.h"

class Gdsfk : public EvictionPolicy {

 public:
  Gdsfk(std::uint32_t s, std::uint8_t k);
  virtual ~Gdsfk();
  void Update(const std::string&, std::uint32_t = 1) override;
  bool IsExist(const std::string&) const override;
  bool IsEmpty() const override;
  std::string Victim() const override;
  std::string ReplaceVictim(const std::string&, std::uint32_t = 1) override;
  void Add(const std::string&, std::uint32_t = 1) override;
  std::string CacheToString() const override;
  void SetFrequencyHistogram(FrequencyHistogram*);

 private:

  typedef DoublyLinkedList<std::string>::Node GdsfkQueryNode;

  const std::uint8_t kFreqExp;
  std::uint64_t aging_factor_;
  std::unordered_map<std::uint64_t, DoublyLinkedList<std::string>*> cache_;
  FrequencyHistogram* freq_hist_;
  std::unordered_map<std::string, std::pair<std::uint64_t,
                                            GdsfkQueryNode*>> cache_table_;
  std::uint64_t max_hval_;

  void Add(GdsfkQueryNode*, std::uint64_t);
  std::uint64_t VictimHVal() const;

  inline std::uint64_t HValue(std::uint32_t freq) const {
    return aging_factor_ + pow(freq, kFreqExp);
  }
};

#endif // CENG778PROJECT_POLICY_EVICTION_GDSFK_H
