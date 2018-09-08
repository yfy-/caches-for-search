// Copyright 2017 folly

#include <cstdio>
#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <thread>
#include "policy/replacement/only_eviction_replacement_policy.h"
#include "policy/eviction/lru_eviction.h"
#include "policy/eviction/inmemory_lfu_eviction.h"
#include "policy/eviction/lfu_eviction.h"
#include "frequency-histogram/pure_lfu_histogram.h"
#include "policy/admission/frequency_admission.h"
#include "policy/replacement/admission_replacement_policy.h"
#include "policy/replacement/admission_controlled_replacement_policy.h"
#include "policy/replacement/static_dynamic_replacement_policy.h"
#include "hitrate_evaluation.h"
#include "frequency-histogram/tiny_lfu_histogram.h"
#include "policy/eviction/inmemory_gdsfk.h"
#include "policy/eviction/gdsfk.h"


void GdsfkEval(const std::vector<std::string>& train,
               const std::vector<std::string>& test, std::uint32_t warmup,
               HitrateEvaluation* const eval, std::uint32_t base_cache_size,
                 std::uint32_t until_multip, std::uint8_t k);

void InmemGdsfkEval(const std::vector<std::string>& test, std::uint32_t warmup,
               HitrateEvaluation* const eval, std::uint32_t base_cache_size,
               std::uint32_t until_multip, std::uint8_t k);

void AcEval(std::uint32_t, const std::vector<std::string>&,
            const std::vector<std::string>&, std::uint32_t, std::uint32_t,
            std::uint32_t);

void FullHistEvEval(const std::vector<std::string>&,
                    const std::vector<std::string>&, std::uint32_t,
                    HitrateEvaluation* const, std::uint32_t, std::uint32_t);

void FullHistAdmEval(const std::vector<std::string>&,
                     const std::vector<std::string>&, std::uint32_t,
                     HitrateEvaluation* const, std::uint32_t, std::uint32_t);

void InMemLfuEval(const std::vector<std::string>&, std::uint32_t,
                  HitrateEvaluation* const, std::uint32_t, std::uint32_t);

void LruEval(const std::vector<std::string>&, std::uint32_t,
             HitrateEvaluation* const, std::uint32_t, std::uint32_t);

void SdcEval(std::uint32_t,
             const std::vector<std::string>&,
             const std::vector<std::string>&,
             HitrateEvaluation* const,
             std::uint32_t,
             std::uint32_t,
             std::uint32_t);

void WindowHistEvEval(const std::vector<std::string>&,
                      const std::vector<std::string>&,
                      std::uint32_t, HitrateEvaluation* const,
                      std::uint32_t, std::uint32_t, std::uint32_t);

int main(int argc, char* argv[]) {
  std::vector<std::string> queries {"a", "b", "c", "c", "a", "a", "c", "d", "a",
        "b", "b", "d", "a", "a", "b", "a", "a", "c"};
  // std::ifstream qtrain_file("../resource/aol_big_train_sorted.txt");

  // if (qtrain_file.is_open()) {
  //   std::string line;

  //   while (getline(qtrain_file, line)) {
  //     queries.push_back(line);
  //   }
  // }
  // qtrain_file.close();

  // std::ifstream qtest_file("../resource/aol_big_test_sorted.txt");

  // if (qtest_file.is_open()) {
  //   std::string line;

  //   while (getline(qtest_file, line)) {
  //     queries.push_back(line);
  //   }
  // }
  // qtest_file.close();

  // std::uint32_t unique_size = 6724891;

  // std::uint32_t total_size = queries.size();
  // std::cout << "Total size " << std::to_string(total_size) << "\n";

  // std::uint32_t train_size =  total_size / 10 * 6;
  // std::cout << "Train size " << std::to_string(train_size) << "\n";

  // std::uint32_t test_size = total_size - train_size;
  // std::cout << "Test size " << std::to_string(test_size) << "\n";

  // std::uint32_t warmup = test_size / 10;
  // std::cout << "Warm-up size " << std::to_string(warmup) << "\n";

  // std::vector<std::string> training_q(queries.begin(),
  //                                     queries.begin() + train_size);
  // std::vector<std::string> test_q(queries.begin() + train_size, queries.end());

  auto lru = new LruEviction(3);
  auto ev = new OnlyEvictionReplacementPolicy(lru, "lru");

  for (auto q : queries) {
    std::cout << "Incoming " << q << std::endl;
    bool res = ev->IsExist(q);
    std::cout << lru->CacheToString() << std::endl << std::endl;
  }

  delete ev;
  delete lru;

  return 0;
}

void FullHistEvEval(const std::vector<std::string>& train,
                    const std::vector<std::string>& test,
                    std::uint32_t warmup, HitrateEvaluation* const eval,
                    std::uint32_t base_cache_size, std::uint32_t until_multip) {

  std::string name = "pure_lfu_long";
  for (std::uint32_t i = 1; i <= until_multip; ++i) {
    auto freq_hist_ev = new PureLfuHistogram();

    std::cout << name << " is training\n";
    // for (auto trq : train) {
    //   freq_hist_ev->Add(trq);
    // }

    std::uint32_t cache_size = base_cache_size * i;
    auto ev_pol2 = new LfuEviction(cache_size, test.size());
    ev_pol2->SetFrequencyHistogram(freq_hist_ev);
    auto lfu_ev = new OnlyEvictionReplacementPolicy(ev_pol2, name);
    std::vector<ReplacementPolicy*> r {lfu_ev};
    std::cout << name << " evaluating, cache: " << std::to_string(cache_size) <<
        "\n";
    eval->Evaluate(test, r, cache_size, warmup);

    delete freq_hist_ev;
    delete ev_pol2;
    delete lfu_ev;
  }
}

void FullHistAdmEval(const std::vector<std::string>& train,
                     const std::vector<std::string>& test,
                     std::uint32_t warmup, HitrateEvaluation* const eval,
                     std::uint32_t base_cache_size,
                     std::uint32_t until_multip) {

  std::string name = "adm_full_hist";
  for (std::uint32_t i = 1; i <= until_multip; ++i) {
    auto freq_hist_ev = new PureLfuHistogram();

    std::cout << name << " is training\n";
    for (auto trq : train) {
      freq_hist_ev->Add(trq);
    }

    std::uint32_t cache_size = base_cache_size * i;
    auto ev_pol2 = new LfuEviction(cache_size, test.size());
    auto adm_pol = new FrequencyAdmission(freq_hist_ev);
    auto lfu_adm = new AdmissionReplacementPolicy(ev_pol2, adm_pol, name);
    std::vector<ReplacementPolicy*> r {lfu_adm};
    std::cout << name << " evaluating, cache: " << std::to_string(cache_size) <<
        "\n";
    eval->Evaluate(test, r, cache_size, warmup);

    delete freq_hist_ev;
    delete ev_pol2;
    delete adm_pol;
    delete lfu_adm;
  }
}

void InMemLfuEval(const std::vector<std::string>& test, std::uint32_t warmup,
                  HitrateEvaluation* const eval, std::uint32_t base_cache_size,
                  std::uint32_t until_multip) {

  std::string name = "inmem_lfu";

  for (std::uint32_t i = 1; i <= until_multip; ++i) {
    std::uint32_t cache_size = base_cache_size * i;
    auto inmem_ev = new InMemoryLfuEviction(cache_size);
    auto inmem_rep_pol = new OnlyEvictionReplacementPolicy(inmem_ev, name);
    std::vector<ReplacementPolicy*> r {inmem_rep_pol};
    std::cout << name << " evaluating, cache: " << std::to_string(cache_size) <<
        "\n";
    eval->Evaluate(test, r, cache_size, warmup);

    delete inmem_ev;
    delete inmem_rep_pol;
  }
}

void LruEval(const std::vector<std::string>& test, std::uint32_t warmup,
             HitrateEvaluation* const eval, std::uint32_t base_cache_size,
             std::uint32_t until_multip) {

  std::string name = "lru";

  for (std::uint32_t i = 1; i <= until_multip; ++i) {
    std::uint32_t cache_size = base_cache_size * i;
    auto lru = new LruEviction(cache_size);
    auto lru_rep_pol = new OnlyEvictionReplacementPolicy(lru, name);
    std::vector<ReplacementPolicy*> r {lru_rep_pol};
    std::cout << name << " evaluating, cache: " << std::to_string(cache_size) <<
        "\n";
    eval->Evaluate(test, r, cache_size, warmup);

    delete lru;
    delete lru_rep_pol;
  }
}

void InmemGdsfkEval(const std::vector<std::string>& test, std::uint32_t warmup,
                    HitrateEvaluation* const eval, std::uint32_t base_cache_size,
                    std::uint32_t until_multip, std::uint8_t k) {

  std::string name = "inmem_gdsf" + std::to_string(k);

  for (std::uint32_t i = 1; i <= until_multip; ++i) {
    std::uint32_t cache_size = base_cache_size * i;
    auto gdsf = new InmemoryGdsfk(cache_size, k);
    auto gdsf_rep_pol = new OnlyEvictionReplacementPolicy(gdsf, name);
    std::vector<ReplacementPolicy*> r {gdsf_rep_pol};
    std::cout << name << " evaluating, cache: " << std::to_string(cache_size) <<
        "\n";
    eval->Evaluate(test, r, cache_size, warmup);

    delete gdsf;
    delete gdsf_rep_pol;
  }
}

void GdsfkEval(const std::vector<std::string>& train,
               const std::vector<std::string>& test, std::uint32_t warmup,
               HitrateEvaluation* const eval, std::uint32_t base_cache_size,
               std::uint32_t until_multip, std::uint8_t k) {

  std::string name = "pure_gdsf" + std::to_string(k);

  for (std::uint32_t i = 1; i <= until_multip; ++i) {
    std::uint32_t cache_size = base_cache_size * i;
    auto pure = new PureLfuHistogram();
    for (auto tq: train)
      pure->Add(tq);
    auto gdsf = new Gdsfk(cache_size, k);
    gdsf->SetFrequencyHistogram(pure);
    auto gdsf_rep_pol = new OnlyEvictionReplacementPolicy(gdsf, name);
    std::vector<ReplacementPolicy*> r {gdsf_rep_pol};
    std::cout << name << " evaluating, cache: " << std::to_string(cache_size) <<
        "\n";
    eval->Evaluate(test, r, cache_size, warmup);

    delete pure;
    delete gdsf;
    delete gdsf_rep_pol;
  }
}

void AcEval(std::uint32_t controlled_prop,
            const std::vector<std::string>& train,
            const std::vector<std::string>& test,
            std::uint32_t base_cache_size,
            std::uint32_t until_multip,
            std::uint32_t warmup) {

  auto exp = new HitrateEvaluation("Cache Size", ",");
  std::string prop = std::to_string(controlled_prop);

  for (std::uint32_t i = 0; i < 21; ++i) {

    std::string freq_threshold = std::to_string(i);
    std::cout << prop << ") Frequency Threshold is " << freq_threshold << "\n";

    for (std::uint32_t j = 1; j <= until_multip; ++j) {
      std::uint32_t cache_size = base_cache_size * j;
      std::cout << prop << ") Cache size is " << std::to_string(cache_size) <<
          "\n";

      std::uint32_t c_size = cache_size / 10 * controlled_prop;
      std::uint32_t u_size = cache_size - c_size;
      std::cout << prop << ") Controlled size is " << std::to_string(c_size) <<
          "\n";
      std::cout << prop << ") Uncontrolled size is " <<
          std::to_string(u_size) << "\n";

      auto freq_hist_adm = new PureLfuHistogram();

      for (auto trq : train) {
        freq_hist_adm->Add(trq);
      }

      auto controlled = new LruEviction(c_size);
      auto uncontrolled = new LruEviction(u_size);
      auto adm = new FrequencyAdmission(freq_hist_adm);
      auto ac = new AdmissionControlledReplacementPolicy(controlled,
                                                         uncontrolled, adm,
                                                         i, "ac_" +
                                                         freq_threshold);

      std::vector<ReplacementPolicy*> r {ac};
      exp->Evaluate(test, r, cache_size, warmup);

      delete controlled;
      delete uncontrolled;
      delete freq_hist_adm;
      delete adm;
      delete ac;
    }
  }

  std::string rest_prop = std::to_string(10 - controlled_prop);
  std::string fname = "aol_full/ac" + prop + "-" + rest_prop + ".csv";
  exp->Dump(fname);
  delete exp;
}

void SdcEval(std::uint32_t static_prop,
             const std::vector<std::string>& train,
             const std::vector<std::string>& test,
             HitrateEvaluation* const eval,
             std::uint32_t base_cache_size,
             std::uint32_t until_multip,
             std::uint32_t warmup) {
  std::string prop = std::to_string(static_prop);

  for (std::uint32_t i = 1; i <= until_multip; ++i) {
    std::uint32_t cache_size = base_cache_size * i;
    std::cout << prop << ") Cache size is " << std::to_string(cache_size) <<
        "\n";

    std::uint32_t st_size = cache_size / 10 * static_prop;
    std::uint32_t dyn_size = cache_size - st_size;

    std::cout << prop << ") Static size is " << std::to_string(st_size) <<
        "\n";
    std::cout << prop << ") Dynamic size is " <<
        std::to_string(dyn_size) << "\n";


    auto lfu = new LfuEviction(st_size, train.size());
    auto pure = new PureLfuHistogram();
    auto gdsfk = new Gdsfk(dyn_size, 3);
    gdsfk->SetFrequencyHistogram(pure);

    auto sdc = new StaticDynamicReplacementPolicy(train, gdsfk, lfu, pure,
                                                 "sdc_" +
                                                 std::to_string(static_prop));
    std::vector<ReplacementPolicy*> r {sdc};
    eval->Evaluate(test, r, cache_size, warmup);

    delete lfu;
    delete pure;
    delete gdsfk;
    delete sdc;
  }
}
