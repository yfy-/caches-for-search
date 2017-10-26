// Copyright 2017 folly

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cache/greedy_dual_size_k_cache.h>
#include "frequency-histogram/tiny_lfu_histogram.h"
#include "frequency-histogram/strawman_histogram.h"
#include "frequency-histogram/pure_lfu_histogram.h"
#include "cache/lfu_cache.h"
#include "cache/lru_cache.h"
#include "cache/random_cache.h"

constexpr std::uint32_t CACHE_SIZE = 67230;
constexpr std::uint32_t WINDOW_SIZE = 17210880;

struct ExperimentResult {
  double hit_rate;
  double avg_resp_t;
};

ExperimentResult Experiment(const std::vector<std::string> &queries, Cache *cache,
                  std::uint32_t warmUp) {
  double hit = 0;
  double miss = 0;
  double total_resp = 0;

  for (auto q : queries) {
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    bool is_hit = cache->IsExist(q);
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> diff = end - start;
    total_resp += diff.count();

    if (warmUp > 0) {
      warmUp--;
      continue;
    }

    if (is_hit)
      hit++;
    else
      miss++;
  }

  return { hit / (hit + miss), total_resp / static_cast<double>(queries.size()) };
}

void WriteExperimentResult(ExperimentResult exp, std::string data,
                           std::string admission, std::string eviction) {
  std::string file_path = "./" + data + "/" + admission + "_" + eviction + ".csv";
  std::ofstream out(file_path, std::ios_base::app);
  if (out.is_open()) {
    out << CACHE_SIZE << ";" << WINDOW_SIZE << ";" << exp.hit_rate <<
        ";" << exp.avg_resp_t << "\n";
  }
  out.close();
}

int main() {
  srand(time(NULL));
  std::vector<std::string> queries;
  std::ifstream q_file("../resource/aol_200K_fixed_sorted.txt");
  if (q_file.is_open()) {
    std::string line;

    while (getline(q_file, line)) {
      queries.push_back(line);
    }
  }
  q_file.close();

  /*
   * * Must know query size and unique query size at compile time.
   * * Because std::bitset requires to.
  */

//  constexpr std::uint32_t query_size = 100;
//  constexpr std::uint32_t unique_size = 62;

  constexpr std::uint32_t query_size = 200000;
  constexpr std::uint32_t unique_size = 134464;
  constexpr std::uint32_t strawman_bit_width = sizeof(WINDOW_SIZE) * 8 - __builtin_clz(WINDOW_SIZE);
  constexpr std::uint32_t warm_up = query_size / 4;
  const std::string data_file = "aol_gdsfk";
  constexpr std::uint32_t wc_ratio = static_cast<std::uint32_t>(
      ceil(WINDOW_SIZE /  static_cast<double>(CACHE_SIZE)));
  constexpr std::uint32_t bit_width_tiny =
      sizeof(wc_ratio) * 8 - __builtin_clz(wc_ratio - 1);

  auto random_cache = new RandomCache(CACHE_SIZE);
  ExperimentResult random_result = Experiment(queries, random_cache, warm_up);
  delete random_cache;
  WriteExperimentResult(random_result, data_file, "none", "random");

  auto strawman_hist =
      new StrawmanHistogram<WINDOW_SIZE, 10, strawman_bit_width>(2);
  random_cache = new RandomCache(CACHE_SIZE);
  random_cache->SetFrequencyHistogram(strawman_hist);
  ExperimentResult random_strawman_result = Experiment(queries, random_cache, warm_up);
  delete strawman_hist;
  delete random_cache;
  WriteExperimentResult(random_strawman_result, data_file, "strawman", "random");

  random_cache = new RandomCache(CACHE_SIZE);
  TinyLfuHistogram<WINDOW_SIZE, bit_width_tiny>* tiny_hist = new TinyLfuHistogram<WINDOW_SIZE, bit_width_tiny>(3);
  random_cache->SetFrequencyHistogram(tiny_hist);
  ExperimentResult random_tiny_result = Experiment(queries, random_cache, warm_up);
  delete tiny_hist;
  delete random_cache;
  WriteExperimentResult(random_tiny_result, data_file, "tiny", "random");

  random_cache = new RandomCache(CACHE_SIZE);
  auto pure_hist = new PureLfuHistogram();
  random_cache->SetFrequencyHistogram(pure_hist);
  ExperimentResult random_pure_result = Experiment(queries, random_cache, warm_up);
  delete pure_hist;
  delete random_cache;
  WriteExperimentResult(random_pure_result, data_file, "pure", "random");

  auto lru_cache = new LruCache(CACHE_SIZE);
  ExperimentResult lru_result = Experiment(queries, lru_cache, warm_up);
  delete lru_cache;
  WriteExperimentResult(lru_result, data_file, "none", "lru");

  strawman_hist =
      new StrawmanHistogram<WINDOW_SIZE, 10, strawman_bit_width>(2);
  lru_cache = new LruCache(CACHE_SIZE);
  lru_cache->SetFrequencyHistogram(strawman_hist);
  ExperimentResult lru_strawman_result = Experiment(queries, lru_cache, warm_up);
  delete strawman_hist;
  delete lru_cache;
  WriteExperimentResult(lru_strawman_result, data_file, "strawman", "lru");

  tiny_hist = new TinyLfuHistogram<WINDOW_SIZE, bit_width_tiny>(3);
  lru_cache = new LruCache(CACHE_SIZE);
  lru_cache->SetFrequencyHistogram(tiny_hist);
  ExperimentResult lru_tiny_result = Experiment(queries, lru_cache, warm_up);
  delete tiny_hist;
  delete lru_cache;
  WriteExperimentResult(lru_tiny_result, data_file, "tiny", "lru");

  pure_hist = new PureLfuHistogram();
  lru_cache = new LruCache(CACHE_SIZE);
  lru_cache->SetFrequencyHistogram(pure_hist);
  ExperimentResult lru_pure_result = Experiment(queries, lru_cache, warm_up);
  delete pure_hist;
  delete lru_cache;
  WriteExperimentResult(lru_pure_result, data_file, "pure", "lru");

  strawman_hist =
      new StrawmanHistogram<WINDOW_SIZE, 10, strawman_bit_width>(2);
  auto lfu_cache = new LfuCache(CACHE_SIZE, strawman_hist);
  ExperimentResult lfu_strawman_result = Experiment(queries, lfu_cache, warm_up);
  delete strawman_hist;
  delete lfu_cache;
  WriteExperimentResult(lfu_strawman_result, data_file, "strawman", "lfu");

  tiny_hist = new TinyLfuHistogram<WINDOW_SIZE, bit_width_tiny>(3);
  lfu_cache = new LfuCache(CACHE_SIZE, tiny_hist);
  ExperimentResult lfu_tiny_result = Experiment(queries, lfu_cache, warm_up);
  delete tiny_hist;
  delete lfu_cache;
  WriteExperimentResult(lfu_tiny_result, data_file, "tiny", "lfu");


  pure_hist = new PureLfuHistogram();
  lfu_cache = new LfuCache(CACHE_SIZE, pure_hist);
  ExperimentResult lfu_pure_result = Experiment(queries, lfu_cache, warm_up);
  delete pure_hist;
  delete lfu_cache;
  WriteExperimentResult(lfu_pure_result, data_file, "pure", "lfu");

  strawman_hist =
      new StrawmanHistogram<WINDOW_SIZE, 10, strawman_bit_width>(2);
  auto gdsfk_cache = new GreedyDualSizeKCache(CACHE_SIZE, strawman_hist);
  ExperimentResult gdsfk_strawman_result = Experiment(queries, gdsfk_cache, warm_up);
  delete strawman_hist;
  delete gdsfk_cache;
  WriteExperimentResult(gdsfk_strawman_result, data_file, "strawman", "gdsfk");

  tiny_hist = new TinyLfuHistogram<WINDOW_SIZE, bit_width_tiny>(3);
  gdsfk_cache = new GreedyDualSizeKCache(CACHE_SIZE, tiny_hist);
  ExperimentResult gdsfk_tiny_result = Experiment(queries, gdsfk_cache, warm_up);
  delete tiny_hist;
  delete gdsfk_cache;
  WriteExperimentResult(gdsfk_tiny_result, data_file, "tiny", "gdsfk");

  pure_hist = new PureLfuHistogram();
  gdsfk_cache = new GreedyDualSizeKCache(CACHE_SIZE, pure_hist);
  ExperimentResult gdsfk_pure_result = Experiment(queries, gdsfk_cache, warm_up);
  delete pure_hist;
  delete gdsfk_cache;
  WriteExperimentResult(gdsfk_pure_result, data_file, "pure", "gdsfk");

  return 0;
}
