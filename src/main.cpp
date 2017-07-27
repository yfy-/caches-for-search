// Copyright 2017 folly

#include <tiny_lfu_histogram.h>
#include <strawman_histogram.h>
#include <pure_lfu_histogram.h>
#include <lfu_cache.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>


double HitRate(const std::vector<std::string> &queries, LfuCache *cache,
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

int main(void) {
  std::vector<std::string> queries;
  std::ifstream q_file("../resource/aol_200K_fixed.txt");

  if (q_file.is_open()) {
    std::string line = "";

    while (getline(q_file, line)) {
      queries.push_back(line);
    }
  }
  q_file.close();

  /*
   * Must know query size and unique query size at compile time.
   * Because std::bitset requires to.
   */

  constexpr std::uint64_t query_size = 200000;
  constexpr std::uint64_t unique_size = 135009;
  constexpr std::uint64_t cache_size = unique_size / 8;
  constexpr std::uint64_t window_size = 500000;
  constexpr std::uint64_t warm_up = query_size / 2;
  constexpr std::uint64_t wc_ratio =
      (std::uint64_t) ceil(window_size / static_cast<double>(cache_size));
  constexpr std::uint64_t bit_width_tiny =
      sizeof(wc_ratio) * 8 - __builtin_clzl(wc_ratio - 1);

  FrequencyHistogram* t_histogram =
      new TinyLfuHistogram<window_size, bit_width_tiny>();
  LfuCache* cache = new LfuCache(cache_size, t_histogram);
  double t_hit_ratio = HitRate(queries, cache, warm_up);

  std::cout << cache_size << ";" << window_size << ";" << warm_up << ";" <<
      t_hit_ratio << std::endl;

  delete t_histogram;
  delete cache;
  return 0;
}
