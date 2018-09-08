// Created by yfy on 14/04/2018

#include "policy/replacement/admission_controlled_replacement_policy.h"

AdmissionControlledReplacementPolicy::AdmissionControlledReplacementPolicy(
    EvictionPolicy* c, EvictionPolicy* u, FrequencyAdmission* a,
    std::uint32_t ft, const std::string& n) : ReplacementPolicy(n),
                                              controlled_{c}, uncontrolled_{u},
                                              admission_{a},
                                              frequency_threshold_{ft} {}

bool AdmissionControlledReplacementPolicy::IsExist(const std::string& query) {
  bool c_exist = controlled_->IsExist(query);
  bool u_exist = uncontrolled_->IsExist(query);

  std::uint32_t estimate = admission_->Estimate(query);

  if (c_exist)
    controlled_->Update(query, estimate);

  if (u_exist)
    uncontrolled_->Update(query, estimate);

  if (!c_exist && !u_exist) {
    if (estimate > frequency_threshold_) {
      if (controlled_->IsEmpty())
        controlled_->Add(query, estimate);
      else
        controlled_->ReplaceVictim(query, estimate);
    } else {
      if (uncontrolled_->IsEmpty())
        uncontrolled_->Add(query, estimate);
      else
        uncontrolled_->ReplaceVictim(query, estimate);
    }
  }

  return c_exist || u_exist;
}

AdmissionControlledReplacementPolicy::~AdmissionControlledReplacementPolicy() {}
