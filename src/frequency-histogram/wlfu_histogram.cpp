#include "wlfu_histogram.h"

std::uint64_t WLfuHistogram::Add(const std::string &query) {
    if (histogram.find(query) != histogram.end()) {
        histogram[query]++;
    } else {
        if (cur_window_size < max_window_size) {
            cur_window_size++;
        } else {
            std::uint64_t min = UINT64_MAX;
            std::string toBeEvicted;
            for ( auto it = histogram.begin(); it != histogram.end(); ++it){
                if (it->second < min){
                    min = it->second;
                    toBeEvicted = it->first;
                }
            }
            histogram.erase(toBeEvicted);
        }
        histogram[query] = 1;
    }

    return histogram[query];
}

std::uint64_t WLfuHistogram::Estimate(const std::string &query) const {
    if (histogram.find(query) != histogram.end())
        return histogram.at(query);
    else
        return 0;
}
