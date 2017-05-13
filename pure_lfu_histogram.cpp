//
// Created by yfy on 5/13/17.
//

#include "pure_lfu_histogram.h"

Uint32 PureLfuHistogram::Add(std::string query) {
    if (histogram.find(query) != histogram.end()) {
        histogram[query]++;
    } else {
        histogram[query] = 1;
    }

    return histogram[query];
}

Uint32 PureLfuHistogram::Estimate(std::string query) {
    return histogram[query];
}
