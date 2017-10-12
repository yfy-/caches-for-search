// Copyright 2017 folly

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <time.h>
#include <chrono>
#include "cache/NodeDoublyLinkedList.h"
#include "frequency-histogram/tiny_lfu_histogram.h"
#include "frequency-histogram/strawman_histogram.h"
#include "frequency-histogram/pure_lfu_histogram.h"
#include "cache/lfu_cache.h"
#include "cache/random_cache.h"

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

int main() {
  srand(time(NULL));
  std::vector<std::string> queries;
  std::ifstream q_file("../resource/aol_200K_fixed.txt");
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

  constexpr std::uint32_t query_size = 200000;
  constexpr std::uint32_t unique_size = 135009;
  constexpr std::uint64_t cache_size = 527;
  constexpr std::uint64_t window_size = 2108;
  constexpr std::uint32_t warm_up = query_size / 4;
  constexpr std::uint32_t wc_ratio = static_cast<std::uint32_t>(
      ceil(window_size /  static_cast<double>(cache_size)));
  constexpr std::uint32_t bit_width_tiny =
      sizeof(wc_ratio) * 8 - __builtin_clz(wc_ratio - 1);

  FrequencyHistogram* tlfu = new TinyLfuHistogram<window_size, bit_width_tiny>(3);
  LfuCache* lfu_cache = new LfuCache(cache_size, tlfu);
  ExperimentResult tlfu_res = Experiment(queries, lfu_cache, warm_up);

  delete tlfu;
  delete lfu_cache;

  std::ofstream out("new_lfu_tiny.csv", std::ios_base::app);
  if (out.is_open()) {
    out << cache_size << ";" << window_size << ";" << tlfu_res.hit_rate <<
        ";" << tlfu_res.avg_resp_t << "\n";
  }
  out.close();
  return 0;
}
