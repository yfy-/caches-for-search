// Created by yfy on 14/04/2018

#ifndef CENG778PROJECT_POLICY_REPLACEMENT_ADMISSIONCONTROLLED_H
#define CENG778PROJECT_POLICY_REPLACEMENT_ADMISSIONCONTROLLED_H

#include "policy/replacement/replacement_policy.h"
#include "policy/eviction/eviction_policy.h"
#include "policy/admission/frequency_admission.h"

class AdmissionControlledReplacementPolicy : public ReplacementPolicy {
 private:
  EvictionPolicy* controlled_;
  EvictionPolicy* uncontrolled_;
  FrequencyAdmission* admission_;
  std::uint32_t frequency_threshold_;

 public:
  AdmissionControlledReplacementPolicy(EvictionPolicy*, EvictionPolicy*,
                                       FrequencyAdmission*, std::uint32_t,
                                       const std::string&);
  bool IsExist(const std::string&) override;
  virtual ~AdmissionControlledReplacementPolicy();
};

#endif // CENG778PROJECT_POLICY_REPLACEMENT_ADMISSIONCONTROLLED_H
