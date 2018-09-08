// Created by yfy on 03/04/2018

#include <fstream>
#include <iostream>
#include "hitrate_evaluation.h"

void HitrateEvaluation::Evaluate(const std::vector<std::string>& queries,
                                 const std::vector<ReplacementPolicy*>& policies,
                                 std::uint32_t respect_to_val,
                                 std::uint32_t warmup, bool progress) {
  std::uint32_t pol_size = policies.size();
  std::uint32_t* hits = new std::uint32_t[pol_size]();
  std::uint32_t* misses = new std::uint32_t[pol_size]();
  std::uint32_t q_size = queries.size();
  std::uint32_t notify_period = q_size / 10;

  for (std::uint32_t q = 0; q < q_size; q++) {
    if (progress) {
      if (q % notify_period == 0)
        std::cout << q / notify_period << "/10 is complete " << std::endl;
    }

    bool warmup_exhaust = warmup == 0;

    for (std::uint32_t i = 0; i < pol_size; ++i) {
      bool is_hit = policies[i]->IsExist(queries[q]);

      if (warmup_exhaust) {
        if (is_hit)
          hits[i]++;
        else
          misses[i]++;
      }
    }

    if (!warmup_exhaust)
      warmup--;
  }

  for (std::uint32_t i = 0; i < pol_size; ++i) {
    double hit = static_cast<double>(hits[i]);
    double hitrate =  hit / (hit + static_cast<double>(misses[i]));
    stats_[respect_to_val][policies[i]->Name()] = hitrate;
  }

  delete hits;
  delete misses;
}

double HitrateEvaluation::GetHitrate(std::uint32_t resp,
                                     const std::string& name) {
  return stats_[resp][name];
}

void HitrateEvaluation::Dump(const std::string& fname) {
  std::ofstream ofs(fname);

  if (ofs.is_open()) {
    ofs << respect_to_ << delimiter_;
    auto rep_names = stats_.begin()->second;

    std::string colnames = "";
    for (auto const& it : rep_names) {
      colnames += it.first + delimiter_;
    }
    ofs << colnames.substr(0, colnames.size() - 1);
    ofs << "\n";

    for (auto const& it1 : stats_) {
      std::string vals = std::to_string(it1.first) + delimiter_;
      for (auto const& it2 : it1.second) {
        vals+= std::to_string(it2.second) + delimiter_;
      }
      ofs << vals.substr(0, vals.size() - 1);
      ofs << "\n";
    }
  }

  ofs.close();
}
