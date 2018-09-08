// Created by yfy on 22/04/2018

#include "policy/replacement/static_dynamic_replacement_policy.h"
#include "policy/replacement/admission_replacement_policy.h"
#include "frequency-histogram/pure_lfu_histogram.h"
#include "policy/admission/frequency_admission.h"

StaticDynamicReplacementPolicy::StaticDynamicReplacementPolicy(
    const std::vector<std::string>& train, EvictionPolicy* dyn, LfuEviction* st,
    FrequencyHistogram* hist, const std::string& n) : ReplacementPolicy(n),
                                                      static_{st}, dynamic_{dyn}
{
  auto freq_adm = new FrequencyAdmission(hist);
  auto adm_pol = new AdmissionReplacementPolicy(static_, freq_adm, "noneed");

  for (auto tq: train)
    adm_pol->IsExist(tq);

  delete adm_pol;
  delete freq_adm;
}

bool StaticDynamicReplacementPolicy::IsExist(const std::string& query) {
  bool s_exist = static_->IsExist(query);
  bool d_exist = dynamic_->IsExist(query);

  if (d_exist)
    dynamic_->Update(query);

  if (!s_exist && !d_exist) {
    if (dynamic_->IsEmpty())
      dynamic_->Add(query);
    else
      dynamic_->ReplaceVictim(query);
  }

  return s_exist || d_exist;
}
