// Copyright 2017 folly

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <time.h>
#include "frequency-histogram/tiny_lfu_histogram.h"
#include "frequency-histogram/strawman_histogram.h"
#include "frequency-histogram/pure_lfu_histogram.h"
#include "cache/lfu_cache.h"
#include "cache/random_cache.h"

double HitRate(const std::vector<std::string> &queries, Cache *cache,
               std::uint64_t warmUp) {
    std::uint64_t hit = 0;
    std::uint64_t miss = 0;

    for (auto q : queries) {
        bool is_hit = cache->IsExist(q);
        if (warmUp > 0) {
            warmUp--;
            continue;
        }

        if (is_hit)
            hit++;
        else
            miss++;
    }

    return hit / static_cast<double>(hit + miss);
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

  constexpr std::uint64_t query_size = 200000;
  constexpr std::uint64_t unique_size = 135009;
	constexpr std::uint64_t cache_size = 33752;
	constexpr std::uint64_t window_size = 135008;
  constexpr std::uint64_t warm_up = query_size / 4;
  constexpr std::uint64_t wc_ratio = static_cast<std::uint64_t>(
      ceil(window_size /  static_cast<double>(cache_size)));
  constexpr std::uint64_t bit_width_tiny =
      sizeof(wc_ratio) * 8 - __builtin_clzl(wc_ratio - 1);

  RandomCache* r_cache = new RandomCache(cache_size);
  double only_random_hr = HitRate(queries, r_cache, warm_up);

  delete r_cache;

  r_cache = new RandomCache(cache_size);
  FrequencyHistogram* tlfu = new TinyLfuHistogram<window_size, bit_width_tiny>(3);
  r_cache->SetFrequencyHistogram(tlfu);
  double random_tlfu = HitRate(queries, r_cache, warm_up);

  delete tlfu;
  delete r_cache;

  std::ofstream out("random-randomTiny.csv", std::ios_base::app);
  if (out.is_open()) {
    out << cache_size << ";" << window_size << ";" << only_random_hr << ";" <<
                                                                           random_tlfu << "\n";
  }
  out.close();
  return 0;
}
