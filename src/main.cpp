// Copyright 2017 folly

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>
#include <cstdio>
#include "doubly_linked_list.h"

// constexpr std::uint32_t CACHE_SIZE = 281450;
// constexpr std::uint32_t WINDOW_SIZE = 8000;
// constexpr std::uint32_t QUERY_SIZE = 1000000;

// struct ExperimentResult {
//   double hit_rate;
//   double avg_resp_t;
// };

// ExperimentResult Experiment(const std::vector<std::string> &queries, Cache *cache,
//                   std::uint32_t warmUp) {
//   double hit = 0;
//   double miss = 0;
//   double total_resp = 0;

//   for (auto q : queries) {
//     std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
//     bool is_hit = cache->IsExist(q);
//     std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double, std::milli> diff = end - start;
//     // std::cout << cache->GetFreqHist_()->ToString() + "\n";
//     // std::cout << cache->ToString() + "\n";

//     total_resp += diff.count();
//     if (warmUp > 0) {
//       warmUp--;
//       continue;
//     }

//     if (is_hit)
//       hit++;
//     else
//       miss++;
//   }
//   return { hit / (hit + miss), total_resp / static_cast<double>(queries.size()) };
// }

// void WriteExperimentResult(ExperimentResult exp, std::string data,
//                            std::string admission, std::string eviction) {
//   std::string file_path = "./" + data + "/" + admission + "_" + eviction + ".csv";
//   std::ofstream out(file_path, std::ios_base::app);
//   if (out.is_open()) {
//     out << CACHE_SIZE << ";" << WINDOW_SIZE << ";" << exp.hit_rate <<
//         ";" << exp.avg_resp_t << "\n";
//   }

//   out.close();
// }

// int main(int argc, char* argv[]) {
//   srand(time(NULL));
//   std::vector<std::string> queries;
//   if (argc == 2) {
//       FILE* infile = fopen("../resource/zipf_2.0_100.bin", "rb");
//       std::uint32_t buffer[QUERY_SIZE];
//       fread(buffer, QUERY_SIZE, sizeof(* buffer), infile);
//       for (int i = 0; i < QUERY_SIZE; ++i) {
//           queries.push_back(std::to_string(buffer[i]));
//       }
//   } else {
//       std::ifstream q_file("../resource/aol_big_train_sorted.txt");
//       std::uint32_t data_size = 1000000;
//       std::uint32_t counter = 0;
//       if (q_file.is_open()) {
//           std::string line;

//           while (getline(q_file, line) && counter < data_size) {
//               queries.push_back(line);
//               counter++;
//           }
//       }
//       q_file.close();
//       std::cout << queries.size();
//   }

//   /*
//    * * Must know query size and unique query size at compile time.
//    * * Because std::bitset requires to.
//   */

//   // constexpr std::uint32_t strawman_bit_width = sizeof(WINDOW_SIZE) * 8 - __builtin_clz(WINDOW_SIZE);
//   constexpr std::uint32_t warm_up = QUERY_SIZE / 2;
//   const std::string data_file = "aol_1m_09_02_2018";
//   constexpr auto wc_ratio = static_cast<std::uint32_t>(
//       ceil(WINDOW_SIZE /  static_cast<double>(CACHE_SIZE)));
//   constexpr std::uint32_t bit_width_tiny =
//       sizeof(wc_ratio) * 8 - __builtin_clz(wc_ratio - 1);
//   constexpr std::uint32_t counter_multip = 16;

//   // strawman_hist = new StrawmanHistogram<WINDOW_SIZE, 10, strawman_bit_width>(2);
//   // lru_cache = new LruCache(CACHE_SIZE);
//   // lru_cache->SetFrequencyHistogram(strawman_hist);
//   // ExperimentResult lru_strawman_result = Experiment(queries, lru_cache, warm_up);
//   // delete strawman_hist;p
//   // delete lru_cache;
//   // WriteExperimentResult(lru_strawman_result, data_file, "strawman", "lru");

//   // auto tiny_hist = new TinyLfuHistogram<CACHE_SIZE * counter_multip, bit_width_tiny>(4, WINDOW_SIZE);
//   // lru_cache = new LruCache(CACHE_SIZE);
//   // lru_cache->SetFrequencyHistogram(tiny_hist);
//   // ExperimentResult res = Experiment(queries, lru_cache, warm_up);
//   // delete tiny_hist;
//   // delete lru_cache;
//   // WriteExperimentResult(lru_tiny_result, data_file, "tinyh4", "lru");

//   // tiny_hist = new TinyLfuHistogram<CACHE_SIZE * counter_multip, bit_width_tiny>(5, WINDOW_SIZE);
//   // lru_cache = new LruCache(CACHE_SIZE);
//   // lru_cache->SetFrequencyHistogram(tiny_hist);
//   // res = Experiment(queries, lru_cache, warm_up);
//   // delete tiny_hist;
//   // delete lru_cache;
//   // WriteExperimentResult(lru_tiny_result, data_file, "tinyh5", "lru");

//   // auto lru_cache = new LruCache(CACHE_SIZE);
//   // ExperimentResult res = Experiment(queries, lru_cache, warm_up);
//   // delete lru_cache;
//   // WriteExperimentResult(res, data_file, "none", "lru");

//   // lru_cache = new LruCache(CACHE_SIZE);
//   // auto pure_hist = new PureLfuHistogram();
//   // lru_cache->SetFrequencyHistogram(pure_hist);
//   // res = Experiment(queries, lru_cache, warm_up);
//   // delete pure_hist;
//   // delete lru_cache;
//   // WriteExperimentResult(res, data_file, "pure", "lru");

//   // auto lfu_cache = new LfuCache(CACHE_SIZE);
//   // res = Experiment(queries, lfu_cache, warm_up);
//   // delete lfu_cache;
//   // WriteExperimentResult(res, data_file, "none", "lfu");

//   // lfu_cache = new LfuCache(CACHE_SIZE);
//   // pure_hist = new PureLfuHistogram();
//   // lfu_cache->SetFrequencyHistogram(pure_hist);
//   // res = Experiment(queries, lfu_cache, warm_up);
//   // delete pure_hist;
//   // delete lfu_cache;
//   // WriteExperimentResult(res, data_file, "pure", "lfu");

//   // auto gdsf_cache = new GreedyDualSizeKCache(CACHE_SIZE, 1);
//   // res = Experiment(queries, gdsf_cache, warm_up);
//   // delete gdsf_cache;
//   // WriteExperimentResult(res, data_file, "none", "gdsf1");

//   // gdsf_cache = new GreedyDualSizeKCache(CACHE_SIZE, 1);
//   // pure_hist = new PureLfuHistogram();
//   // gdsf_cache->SetFrequencyHistogram(pure_hist);
//   // res = Experiment(queries, gdsf_cache, warm_up);
//   // delete pure_hist;
//   // delete gdsf_cache;
//   // WriteExperimentResult(res, data_file, "pure", "gdsf1");

//   // gdsf_cache = new GreedyDualSizeKCache(CACHE_SIZE, 2);
//   // res = Experiment(queries, gdsf_cache, warm_up);
//   // delete gdsf_cache;
//   // WriteExperimentResult(res, data_file, "none", "gdsf2");

//   // gdsf_cache = new GreedyDualSizeKCache(CACHE_SIZE, 2);
//   // pure_hist = new PureLfuHistogram();
//   // gdsf_cache->SetFrequencyHistogram(pure_hist);
//   // res = Experiment(queries, gdsf_cache, warm_up);
//   // delete pure_hist;
//   // delete gdsf_cache;
//   // WriteExperimentResult(res, data_file, "pure", "gdsf2");

//   // auto gdsf_cache = new GreedyDualSizeKCache(CACHE_SIZE, 3);
//   // ExperimentResult res = Experiment(queries, gdsf_cache, warm_up);
//   // delete gdsf_cache;
//   // WriteExperimentResult(res, data_file, "none", "gdsf_pr");

//   // gdsf_cache = new GreedyDualSizeKCache(CACHE_SIZE, 3);
//   // auto tiny_hist = new TinyLfuHistogram<CACHE_SIZE * counter_multip, bit_width_tiny>(5, WINDOW_SIZE);
//   // gdsf_cache->SetFrequencyHistogram(tiny_hist);
//   // ExperimentResult res = Experiment(queries, gdsf_cache, warm_up);
//   // delete tiny_hist;
//   // delete gdsf_cache;
//   // WriteExperimentResult(res, data_file, "tiny", "gdsf_pr");

//   // gdsf_cache = new GreedyDualSizeKCache(CACHE_SIZE, 3);
//   // auto pure_hist = new PureLfuHistogram();
//   // gdsf_cache->SetFrequencyHistogram(pure_hist);
//   // res = Experiment(queries, gdsf_cache, warm_up);
//   // delete pure_hist;
//   // delete gdsf_cache;
//   // WriteExperimentResult(res, data_file, "pure", "gdsf_pr");

//   auto lru = new LruCache(CACHE_SIZE);
//   ExperimentResult res = Experiment(queries, lru, warm_up);
//   delete lru;
//   WriteExperimentResult(res, data_file, "none", "lru");

//   auto lfu = new LfuCache(CACHE_SIZE);
//   res = Experiment(queries, lfu, warm_up);
//   delete lfu;
//   WriteExperimentResult(res, data_file, "none", "lfu");

//   return 0;
// }

int main(int argc, char* argv[]) {
  struct SomeStrct {
    std::uint32_t d;
    SomeStrct(const std::uint32_t& data) : d{data} {}
  };

  typedef DoublyLinkedList<SomeStrct*> UintList;
  typedef UintList::Node UintNode;

  auto x1 = new UintNode(new SomeStrct(1));
  auto x2 = new UintNode(new SomeStrct(2));
  auto x3 = new UintNode(new SomeStrct(3));
  auto list = new UintList();

  list->PushFront(x1);
  list->PushFront(x2);
  list->PushBack(x3);
  list->MoveAfter(x2, x1);
  list->MoveAfter(x1, x3);
  // list->MoveToFront(x1);
  UintNode* crr = list->head_->next;

  while (crr != list->tail_) {
    std::cout << std::to_string(crr->content->d) << "\n";
    crr = crr->next;
  }

  return 0;
}
