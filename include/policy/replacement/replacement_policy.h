// Created by yfy on 28/03/2018

#ifndef CENG778PROJECT_POLICY_REPLACEMENT_REPLACEMENTPOLICY_H
#define CENG778PROJECT_POLICY_REPLACEMENT_REPLACEMENTPOLICY_H

#include <string>

class ReplacementPolicy {
 public:

  virtual bool IsExist(const std::string&) = 0;
  ReplacementPolicy(const std::string& s) : name_{s} {}
  inline std::string Name() { return name_; }

 private:

  std::string name_;
};

#endif // CENG778PROJECT_POLICY_REPLACEMENT_REPLACEMENTPOLICY_H
