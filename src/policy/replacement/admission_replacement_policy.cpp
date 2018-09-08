// Created by yfy on 01/04/2018

#include "policy/replacement/admission_replacement_policy.h"

AdmissionReplacementPolicy::AdmissionReplacementPolicy(EvictionPolicy* e,
                                                       FrequencyAdmission* a,
                                                       const std::string& n) :
    ReplacementPolicy(n), eviction_{e}, admission_{a}, victim_kept_{0} {}

bool AdmissionReplacementPolicy::IsExist(const std::string& query) {
  bool exist = eviction_->IsExist(query);

  std::uint32_t estimate = admission_->Add(query);

  if (exist) {
    eviction_->Update(query, estimate);
  } else {
    if (eviction_->IsEmpty()) {
      eviction_->Add(query, estimate);
    } else {
      std::uint32_t victim_est = admission_->Estimate(eviction_->Victim());
      if (estimate >= victim_est)
        eviction_->ReplaceVictim(query, estimate);
      else
        victim_kept_++;
    }
  }

  return exist;
}

AdmissionReplacementPolicy::~AdmissionReplacementPolicy() {}
