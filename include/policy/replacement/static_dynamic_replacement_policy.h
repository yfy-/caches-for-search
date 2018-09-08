// Created by yfy on 22/04/2018

#ifndef CENG778PROJECT_POLICY_REPLACEMENT_STATICDYNAMICREPLACEMENTPOLICY_H
#define CENG778PROJECT_POLICY_REPLACEMENT_STATICDYNAMICREPLACEMENTPOLICY_H

#include <vector>
#include <string>
#include "policy/replacement/replacement_policy.h"
#include "policy/eviction/eviction_policy.h"
#include "policy/eviction/lfu_eviction.h"


class StaticDynamicReplacementPolicy : public ReplacementPolicy {
 public:
  StaticDynamicReplacementPolicy(const std::vector<std::string>&,
                                 EvictionPolicy*, LfuEviction*,
                                 FrequencyHistogram*, const std::string&);
  bool IsExist(const std::string&) override;
  virtual ~StaticDynamicReplacementPolicy() {};

 private:
  LfuEviction* static_;
  EvictionPolicy* dynamic_;
};

#endif // CENG778PROJECT_POLICY_REPLACEMENT_STATICDYNAMICREPLACEMENTPOLICY_H
