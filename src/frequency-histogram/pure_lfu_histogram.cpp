//
// Created by yfy on 5/13/17.
//

#include "pure_lfu_histogram.h"

std::uint64_t PureLfuHistogram::Add(const std::string &query) {
    if (histogram.find(query) != histogram.end()) {
        histogram[query]++;
    } else {
        histogram[query] = 1;
    }

    return histogram[query];
}

std::uint64_t PureLfuHistogram::Estimate(const std::string &query) {
    return histogram[query];
}
