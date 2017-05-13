#include <iostream>
#include <fstream>
#include <vector>
#include "frequency_histogram.h"
#include "pure_lfu_histogram.h"
#include "lfu_cache.h"
#include "util.h"

int main(void) {

    std::vector<std::string> queries;
    std::ifstream q_file ("query_log.txt");

    if (q_file.is_open()) {
        std::string line = "";
        while (getline(q_file, line)) {
            std::vector<std::string> tokens = utl::split(line, ':');
            queries.push_back(tokens[1]);
        }
    }
    q_file.close();

    std::cout << "Query Size " << queries.size() << std::endl;
    Uint32 unique_size = utl::unique_size(queries);
    std::cout << "Unique Size " << unique_size << std::endl;

    FrequencyHistogram* pure = new PureLfuHistogram();

    // Cache size is 10% of unique query size
    LfuCache* cache = new LfuCache(unique_size / 10, pure);

    // Warm up is done by first 20% of query size
    Uint32 warm_up = (Uint32) (queries.size() / 20);
    
    Uint32 hit = 0;
    Uint32 miss = 0;
    for (auto q: queries) {
        bool is_hit = cache->IsExist(q);
        if (warm_up > 0) {
            warm_up--;
            continue;
        }

        if (is_hit)
            hit++;
        else
            miss++;
    }

    std::cout << "Hit " << hit << "Miss" << miss << std::endl;
    delete pure;
    delete cache;
    return 0;
}