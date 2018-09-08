// Created by yfy on 28/03/2018

#ifndef CENG778PROJECT_POLICY_REPLACEMENT_ONLYEVICTIONREPLACEMENTPOLICY_H
#define CENG778PROJECT_POLICY_REPLACEMENT_ONLYEVICTIONREPLACEMENTPOLICY_H

#include "policy/replacement/replacement_policy.h"
#include "policy/eviction/eviction_policy.h"

class OnlyEvictionReplacementPolicy : public ReplacementPolicy {

 private:
  EvictionPolicy* eviction_;

 public:
  OnlyEvictionReplacementPolicy(EvictionPolicy*, const std::string&);
  virtual ~OnlyEvictionReplacementPolicy();
  bool IsExist(const std::string&) override;
};

#endif // CENG778PROJECT_POLICY_REPLACEMENT_ONLYEVICTIONREPLACEMENTPOLICY_H
