// Created by yfy on 01/04/2018

#ifndef CENG778PROJECT_POLICY_REPLACEMENT_ADMISSIONREPLACEMENTPOLICY_H
#define CENG778PROJECT_POLICY_REPLACEMENT_ADMISSIONREPLACEMENTPOLICY_H

#include "policy/replacement/replacement_policy.h"
#include "policy/eviction/eviction_policy.h"
#include "policy/admission/frequency_admission.h"

class AdmissionReplacementPolicy : public ReplacementPolicy {
 private:
  EvictionPolicy* eviction_;
  FrequencyAdmission* admission_;
  std::uint32_t victim_kept_;

 public:
  AdmissionReplacementPolicy(EvictionPolicy*, FrequencyAdmission*,
                             const std::string&);
  bool IsExist(const std::string&) override;
  inline std::uint32_t VictimKept() { return victim_kept_; }
  virtual ~AdmissionReplacementPolicy();
};

#endif // CENG778PROJECT_POLICY_REPLACEMENT_ADMISSIONREPLACEMENTPOLICY_H
