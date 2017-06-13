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

    /*
     * Must know query size and unique query size at compile time.
     * Because std::bitset requires to.
     */
    std::ofstream o_file ("output.csv", std::ios_base::app);

    constexpr std::uint64_t query_size = 200000;
    constexpr std::uint64_t unique_size = 135009;

	constexpr std::uint64_t cache_size = 131;
	constexpr std::uint64_t window_size = 195;
	constexpr std::uint64_t warm_up = 24960;

    constexpr std::uint64_t wc_ratio = (std::uint64_t) ceil(window_size / (double) cache_size);
    constexpr std::uint64_t bit_width_tiny = sizeof(wc_ratio) * 8 - __builtin_clzl(wc_ratio - 1);

    FrequencyHistogram* t_histogram = new TinyLfuHistogram<window_size, bit_width_tiny>();
    LfuCache* cache = new LfuCache(cache_size, t_histogram);
    double tiny_hit_ratio = MissRatio(queries, cache, warm_up);

    delete t_histogram;
    delete cache;

    constexpr std::uint64_t n_segments = 10;
    constexpr std::uint64_t wl_ratio = (std::uint64_t) ceil(window_size / (double) n_segments);
    constexpr std::uint64_t bit_witdh_straw = sizeof(wl_ratio) * 8 - __builtin_clzl(wl_ratio);

    FrequencyHistogram* s_histogram = new StrawmanHistogram<window_size, n_segments, bit_witdh_straw>();

    cache = new LfuCache(cache_size, s_histogram);
    double strawman_hit_ratio = MissRatio(queries, cache, warm_up);

    o_file << cache_size << ";" << window_size << ";" << warm_up << ";" << bit_width_tiny << ";" <<
           bit_witdh_straw << ";" << tiny_hit_ratio << ";" << strawman_hit_ratio << std::endl;

    o_file.close();
    
    delete s_histogram;
    delete cache;
    return 0;
}
