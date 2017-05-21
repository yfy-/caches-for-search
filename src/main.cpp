#include <iostream>
#include <fstream>
#include <vector>
#include "frequency-histogram/frequency_histogram.h"
#include "frequency-histogram/pure_lfu_histogram.h"
#include "lfu_cache.h"
#include "util.h"
#include "hash/murmur3.h"
#include "hash/fnv1a.h"
#include "frequency-histogram/spectral_bloom_filter.h"


int main(void) {

//    std::vector<std::string> queries;
//    std::ifstream q_file ("query_log.txt");
//
//    if (q_file.is_open()) {
//        std::string line = "";
//        while (getline(q_file, line)) {
//            std::vector<std::string> tokens = utl::split(line, ':');
//            queries.push_back(tokens[1]);
//        }
//    }
//    q_file.close();
//
//    std::cout << "Query Size " << queries.size() << std::endl;
//    std::uint64_t unique_size = utl::unique_size(queries);
//    std::cout << "Unique Size " << unique_size << std::endl;
//
//    FrequencyHistogram* pure = new PureLfuHistogram();
//
//    // Cache size is 10% of unique query size
//    LfuCache* cache = new LfuCache(unique_size / 10, pure);
//
//    // Warm up is done by first 20% of query size
//    std::uint64_t warm_up = (std::uint64_t) (queries.size() / 20);
//
//    std::uint64_t hit = 0;
//    std::uint64_t miss = 0;
//    for (auto q: queries) {
//        bool is_hit = cache->IsExist(q);
//        if (warm_up > 0) {
//            warm_up--;
//            continue;
//        }
//
//        if (is_hit)
//            hit++;
//        else
//            miss++;
//    }
//
//    std::cout << "Hit " << hit << "Miss" << miss << std::endl;
//    delete pure;
//    delete cache;

//    SpectralBloomFilter sbf<5,5>;
//
//    std::cout << sizeof(sbf) << std::endl;
//
//    sbf.Add(std::vector<std::uint64_t> {0});
//    sbf.Add(std::vector<std::uint64_t> {0});
//    sbf.Add(std::vector<std::uint64_t> {0});
//    sbf.Add(std::vector<std::uint64_t> {0});
//    sbf.Add(std::vector<std::uint64_t> {0});
//
//    std::cout << sbf.ToString()<< std::endl;
//
//    sbf.Reset();
//
//    std::cout << sbf.ToString()<< std::endl;
//    std::cout << sbf.Estimate(std::vector<std::uint64_t> {0, 1, 2});

    std::cout << 5 / (double) 2 - 1;

    return 0;
}