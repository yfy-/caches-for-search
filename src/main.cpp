// Copyright 2017 folly

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <time.h>
#include "cache/NodeDoublyLinkedList.h"
#include "frequency-histogram/tiny_lfu_histogram.h"
#include "frequency-histogram/strawman_histogram.h"
#include "frequency-histogram/pure_lfu_histogram.h"
#include "cache/lfu_cache.h"
#include "cache/random_cache.h"

double HitRate(const std::vector<std::string> &queries, Cache *cache,
               std::uint32_t warmUp) {
    std::uint32_t hit = 0;
    std::uint32_t miss = 0;

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
  /*srand(time(NULL));
  std::vector<std::string> queries;
  std::ifstream q_file("../resource/aol_200K_fixed.txt");
  if (q_file.is_open()) {
    std::string line;

    while (getline(q_file, line)) {
      queries.PushBack(line);
    }
  }
  q_file.close();*/

  /*
   * * Must know query size and unique query size at compile time.
   * * Because std::bitset requires to.
  */

/*
  constexpr std::uint32_t query_size = 200000;
  constexpr std::uint32_t unique_size = 135009;
	constexpr std::uint32_t cache_size = 33752;
	constexpr std::uint32_t window_size = 135008;
  constexpr std::uint32_t warm_up = query_size / 4;
  constexpr std::uint32_t wc_ratio = static_cast<std::uint32_t>(
      ceil(window_size /  static_cast<double>(cache_size)));
  constexpr std::uint32_t bit_width_tiny =
      sizeof(wc_ratio) * 8 - __builtin_clz(wc_ratio - 1);

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
*/

  NodeDoublyLinkedList* list = new NodeDoublyLinkedList();
  NodeDoublyLinkedList::Node* node1 = new NodeDoublyLinkedList::Node("1");
  NodeDoublyLinkedList::Node* node2 = new NodeDoublyLinkedList::Node("2");
  NodeDoublyLinkedList::Node* node3 = new NodeDoublyLinkedList::Node("3");
  NodeDoublyLinkedList::Node* node4 = new NodeDoublyLinkedList::Node("4");
  list->PushBack(node1);
  list->PushFront(node2);
  list->PushBack(node3);
  list->PushFront(node4);
  std::cout << list->ToString() << "\n";
  list->MoveAfter(node4, node2);
  std::cout << list->ToString() << "\n";
  list->MoveAfter(node4, node3);
  std::cout << list->ToString() << "\n";
  list->MoveBefore(node4, node2);
  std::cout << list->ToString() << "\n";
  delete list;
  return 0;
}
