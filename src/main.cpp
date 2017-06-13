#include <iostream>
#include <fstream>
#include <vector>
#include <tiny_lfu_histogram.h>
#include <algorithm>
#include <strawman_histogram.h>
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
     * Because std::bitset requires to.
     */
    constexpr std::uint64_t query_size = 200000;
    constexpr std::uint64_t unique_size = 135009;
    constexpr std::uint64_t cache_size = unique_size / 16;

    constexpr std::uint64_t window_size = query_size / 4;
    constexpr std::uint64_t warm_up = 2 * window_size;

    constexpr std::uint64_t wc_ratio = (std::uint64_t) ceil(window_size / (double) cache_size);
    constexpr std::uint64_t bit_width_tiny = sizeof(wc_ratio) * 8 - __builtin_clzl(wc_ratio - 1);

	constexpr std::uint64_t n_segments = 50;

    std::cout << "Segment count = " + n_segments << std::endl;
    constexpr std::uint64_t wl_ratio = (std::uint64_t) ceil(window_size / (double) n_segments);
    constexpr std::uint64_t bit_witdh_straw = sizeof(wl_ratio) * 8 - __builtin_clzl(wl_ratio);

    std::cout << "Bit width of Strawman = " << bit_witdh_straw << std::endl;
    FrequencyHistogram* histogram = new StrawmanHistogram<window_size, n_segments, bit_witdh_straw>();
    LfuCache* cache = new LfuCache(cache_size, histogram);

    std::cout << "Strawman = " << MissRatio(queries, cache, warm_up) << std::endl;

    delete histogram;
    delete cache;
    return 0;
}
