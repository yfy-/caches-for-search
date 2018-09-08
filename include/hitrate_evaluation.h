// Created by yfy on 03/04/2018

#ifndef CENG778PROJECT_HITRATEEVALUATION_H
#define CENG778PROJECT_HITRATEEVALUATION_H

#include <string>
#include <map>
#include <vector>
#include "policy/replacement/replacement_policy.h"

class HitrateEvaluation {
 public:

  HitrateEvaluation(const std::string& r, const std::string& d) :
      respect_to_{r},  delimiter_{d} {}
  void Evaluate(const std::vector<std::string>&,
                const std::vector<ReplacementPolicy*>&,
                std::uint32_t, std::uint32_t, bool = false);
  double GetHitrate(std::uint32_t, const std::string&);
  void Dump(const std::string&);

 private:

  std::string respect_to_;
  std::string delimiter_;
  std::map<std::uint32_t, std::map<std::string, double>> stats_;
};

#endif // CENG778PROJECT_HITRATEEVALUATION_H
