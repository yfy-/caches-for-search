// Copyright 2017 folly

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>
#include <cstdio>
#include "frequency-histogram/window_lfu_histogram.h"
#include "frequency-histogram/tiny_lfu_histogram.h"
#include "frequency-histogram/strawman_histogram.h"
#include "frequency-histogram/pure_lfu_histogram.h"
#include "cache/lfu_cache.h"
#include "cache/lru_cache.h"
#include "cache/random_cache.h"
#include "cache/greedy_dual_size_k_cache.h"

constexpr std::uint32_t CACHE_SIZE = 121008;
constexpr std::uint32_t WINDOW_SIZE = 484032;
constexpr std::uint32_t QUERY_SIZE = 2000000;

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

int main(int argc, char* argv[]) {
  srand(time(NULL));
  std::vector<std::string> queries;
  if (argc == 2) {
      FILE* infile = fopen("../resource/zipf_0.9_2m.bin", "rb");
      std::uint32_t buffer[QUERY_SIZE];
      fread(buffer, QUERY_SIZE, sizeof(* buffer), infile);
      for (int i = 0; i < QUERY_SIZE; ++i) {
          queries.push_back(std::to_string(buffer[i]));
      }
  } else {
      std::ifstream q_file("../resource/aol_200K_fixed_sorted.txt");
      if (q_file.is_open()) {
          std::string line;

          while (getline(q_file, line)) {
              queries.push_back(line);
          }
      }
      q_file.close();
  }

  /*
   * * Must know query size and unique query size at compile time.
   * * Because std::bitset requires to.
  */

  constexpr std::uint32_t strawman_bit_width = sizeof(WINDOW_SIZE) * 8 - __builtin_clz(WINDOW_SIZE);
  constexpr std::uint32_t warm_up = QUERY_SIZE / 4;
  const std::string data_file = "zipf-0.9-2m";
  constexpr auto wc_ratio = static_cast<std::uint32_t>(
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
  auto tiny_hist = new TinyLfuHistogram<WINDOW_SIZE, bit_width_tiny>(3);
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

  auto window_hist = new WindowLfuHistogram(WINDOW_SIZE);
  random_cache = new RandomCache(CACHE_SIZE);
  random_cache->SetFrequencyHistogram(window_hist);
  ExperimentResult random_window_result = Experiment(queries, random_cache, warm_up);
  delete window_hist;
  delete random_cache;
  WriteExperimentResult(random_window_result, data_file, "window", "random");

  auto lru_cache = new LruCache(CACHE_SIZE);
  ExperimentResult lru_result = Experiment(queries, lru_cache, warm_up);
  delete lru_cache;
  WriteExperimentResult(lru_result, data_file, "none", "lru");

  strawman_hist = new StrawmanHistogram<WINDOW_SIZE, 10, strawman_bit_width>(2);
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

  window_hist = new WindowLfuHistogram(WINDOW_SIZE);
  lru_cache = new LruCache(CACHE_SIZE);
  lru_cache->SetFrequencyHistogram(window_hist);
  ExperimentResult lru_window_result = Experiment(queries, lru_cache, warm_up);
  delete window_hist;
  delete lru_cache;
  WriteExperimentResult(lru_window_result, data_file, "window", "lru");

  strawman_hist = new StrawmanHistogram<WINDOW_SIZE, 10, strawman_bit_width>(2);
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

  window_hist = new WindowLfuHistogram(WINDOW_SIZE);
  lfu_cache = new LfuCache(CACHE_SIZE, window_hist);
  ExperimentResult lfu_window_result = Experiment(queries, lfu_cache, warm_up);
  delete window_hist;
  delete lfu_cache;
  WriteExperimentResult(lfu_window_result, data_file, "window", "lfu");

  strawman_hist = new StrawmanHistogram<WINDOW_SIZE, 10, strawman_bit_width>(2);
  auto gdsf1_cache = new GreedyDualSizeKCache(CACHE_SIZE, 1, strawman_hist);
  ExperimentResult gdsf1_strawman_result = Experiment(queries, gdsf1_cache, warm_up);
  delete strawman_hist;
  delete gdsf1_cache;
  WriteExperimentResult(gdsf1_strawman_result, data_file, "strawman", "gdsf1");

  tiny_hist = new TinyLfuHistogram<WINDOW_SIZE, bit_width_tiny>(3);
  gdsf1_cache = new GreedyDualSizeKCache(CACHE_SIZE, 1, tiny_hist);
  ExperimentResult gdsf1_tiny_result = Experiment(queries, gdsf1_cache, warm_up);
  delete tiny_hist;
  delete gdsf1_cache;
  WriteExperimentResult(gdsf1_tiny_result, data_file, "tiny", "gdsf1");

  pure_hist = new PureLfuHistogram();
  gdsf1_cache = new GreedyDualSizeKCache(CACHE_SIZE, 1, pure_hist);
  ExperimentResult gdsf1_pure_result = Experiment(queries, gdsf1_cache, warm_up);
  delete pure_hist;
  delete gdsf1_cache;
  WriteExperimentResult(gdsf1_pure_result, data_file, "pure", "gdsf1");

  window_hist = new WindowLfuHistogram(WINDOW_SIZE);
  gdsf1_cache = new GreedyDualSizeKCache(CACHE_SIZE, 1, window_hist);
  ExperimentResult gdsf1_window_result = Experiment(queries, gdsf1_cache, warm_up);
  delete window_hist;
  delete gdsf1_cache;
  WriteExperimentResult(gdsf1_window_result, data_file, "window", "gdsf1");

  strawman_hist = new StrawmanHistogram<WINDOW_SIZE, 10, strawman_bit_width>(2);
  auto gdsf2_cache = new GreedyDualSizeKCache(CACHE_SIZE, 2, strawman_hist);
  ExperimentResult gdsf2_strawman_result = Experiment(queries, gdsf2_cache, warm_up);
  delete strawman_hist;
  delete gdsf2_cache;
  WriteExperimentResult(gdsf2_strawman_result, data_file, "strawman", "gdsf2");

  tiny_hist = new TinyLfuHistogram<WINDOW_SIZE, bit_width_tiny>(3);
  gdsf2_cache = new GreedyDualSizeKCache(CACHE_SIZE, 2, tiny_hist);
  ExperimentResult gdsf2_tiny_result = Experiment(queries, gdsf2_cache, warm_up);
  delete tiny_hist;
  delete gdsf2_cache;
  WriteExperimentResult(gdsf2_tiny_result, data_file, "tiny", "gdsf2");

  pure_hist = new PureLfuHistogram();
  gdsf2_cache = new GreedyDualSizeKCache(CACHE_SIZE, 2, pure_hist);
  ExperimentResult gdsf2_pure_result = Experiment(queries, gdsf2_cache, warm_up);
  delete pure_hist;
  delete gdsf2_cache;
  WriteExperimentResult(gdsf2_pure_result, data_file, "pure", "gdsf2");

  window_hist = new WindowLfuHistogram(WINDOW_SIZE);
  gdsf2_cache = new GreedyDualSizeKCache(CACHE_SIZE, 2, window_hist);
  ExperimentResult gdsf2_window_result = Experiment(queries, gdsf2_cache, warm_up);
  delete window_hist;
  delete gdsf2_cache;
  WriteExperimentResult(gdsf2_window_result, data_file, "window", "gdsf2");

  strawman_hist = new StrawmanHistogram<WINDOW_SIZE, 10, strawman_bit_width>(2);
  auto gdsf3_cache = new GreedyDualSizeKCache(CACHE_SIZE, 3, strawman_hist);
  ExperimentResult gdsf3_strawman_result = Experiment(queries, gdsf3_cache, warm_up);
  delete strawman_hist;
  delete gdsf3_cache;
  WriteExperimentResult(gdsf3_strawman_result, data_file, "strawman", "gdsf3");

  tiny_hist = new TinyLfuHistogram<WINDOW_SIZE, bit_width_tiny>(3);
  gdsf3_cache = new GreedyDualSizeKCache(CACHE_SIZE, 3, tiny_hist);
  ExperimentResult gdsf3_tiny_result = Experiment(queries, gdsf3_cache, warm_up);
  delete tiny_hist;
  delete gdsf3_cache;
  WriteExperimentResult(gdsf3_tiny_result, data_file, "tiny", "gdsf3");

  pure_hist = new PureLfuHistogram();
  gdsf3_cache = new GreedyDualSizeKCache(CACHE_SIZE, 3, pure_hist);
  ExperimentResult gdsf3_pure_result = Experiment(queries, gdsf3_cache, warm_up);
  delete pure_hist;
  delete gdsf3_cache;
  WriteExperimentResult(gdsf3_pure_result, data_file, "pure", "gdsf3");

  window_hist = new WindowLfuHistogram(WINDOW_SIZE);
  gdsf3_cache = new GreedyDualSizeKCache(CACHE_SIZE, 3, window_hist);
  ExperimentResult gdsf3_window_result = Experiment(queries, gdsf3_cache, warm_up);
  delete window_hist;
  delete gdsf3_cache;
  WriteExperimentResult(gdsf3_window_result, data_file, "window", "gdsf3");

  return 0;
}
