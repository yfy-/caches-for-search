// Created by yfy on 01/04/2018

#ifndef CENG778PROJECT_POLICY_ADMISSION_FREQUENCYADMISSION_H
#define CENG778PROJECT_POLICY_ADMISSION_FREQUENCYADMISSION_H

#include "policy/admission/admission_policy.h"
#include "frequency-histogram/frequency_histogram.h"

class FrequencyAdmission : public AdmissionPolicy<std::uint32_t> {

 public:
  FrequencyAdmission(FrequencyHistogram* fh) : freq_hist_{fh} {}

  inline std::uint32_t Add(const std::string& query) override {
    return freq_hist_->Add(query);
  }

  std::uint32_t Estimate(const std::string& query) const override {
    return freq_hist_->Estimate(query);
  }

  virtual ~FrequencyAdmission() {};

 private:
  FrequencyHistogram* freq_hist_;
};


#endif // CENG778PROJECT_POLICY_ADMISSION_FREQUENCYADMISSION_H
