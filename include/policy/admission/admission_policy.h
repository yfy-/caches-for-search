// Created by yfy on 28/03/2018

#ifndef CENG778PROJECT_POLICY_ADMISSION_ADMISSIONPOLICY_H
#define CENG778PROJECT_POLICY_ADMISSION_ADMISSIONPOLICY_H

#include <string>

template <class T>
class AdmissionPolicy {

 public:

  virtual T Add(const std::string&) = 0;
  virtual T Estimate(const std::string &) const = 0;
};

#endif // CENG778PROJECT_POLICY_ADMISSION_ADMISSIONPOLICY_H
