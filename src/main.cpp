#include <iostream>
#include <fstream>
#include <vector>
#include <tiny_lfu_histogram.h>
#include <algorithm>
#include "pure_lfu_histogram.h"
#include "lfu_cache.h"

double MissRatio(const std::vector<std::string> &queries, LfuCache* cache, std::uint64_t warmUp) {
    std::uint64_t hit = 0;
    std::uint64_t miss = 0;


    for (auto q: queries) {
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

    return hit / ( (double) hit + miss);
}

int main(void) {
    std::vector<std::string> queries;
    std::ifstream q_file ("../resource/aol_200K_fixed.txt");

    if (q_file.is_open()) {
        std::string line = "";
        while (getline(q_file, line)) {
            queries.push_back(line);
        }
    }
    q_file.close();

//    std::cout << queries.size() << std::endl;
//    std::cout << utl::unique_size(queries) << std::endl;

    /*
     * Must know query size and unique query size at compile time.
     * Because std::bitset requires to
     */
    constexpr std::uint64_t query_size = 200000;
    constexpr std::uint64_t unique_size = 135009;
    constexpr std::uint64_t cache_size = unique_size / 8;
    constexpr std::uint64_t window_size = query_size / 2;
    constexpr std::uint64_t wc_ratio = (std::uint64_t) ceil(window_size / (double) cache_size);
    constexpr std::uint64_t bit_width = sizeof(wc_ratio) * 8 - __builtin_clzl(wc_ratio - 1);
    constexpr std::uint64_t warm_up = window_size;

    std::cout << "Bit width = " << bit_width << std::endl;
    FrequencyHistogram* t_histogram = new TinyLfuHistogram<window_size, bit_width>();
    LfuCache* cache = new LfuCache(cache_size, t_histogram);
    std::cout << "Tiny LFU = " << MissRatio(queries, cache, warm_up) << std::endl;
    std::cout << "Tiny LFU Memory = " << sizeof(*t_histogram) << std::endl;

    delete t_histogram;
    delete cache;

//    FrequencyHistogram* p_histogram = new PureLfuHistogram();
//
//    cache = new LfuCache(cache_size, p_histogram);
//
//    std::cout << "Pure LFU = " << MissRatio(queries, cache, warm_up) << std::endl;
//    std::cout << "Pure LFU Memory = " << sizeof(*p_histogram) << std::endl;
//
//    delete p_histogram;
//    delete cache;
    return 0;
}
