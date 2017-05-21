#include "wlfu_histogram.h"

Uint32 WLfuHistogram::Add(std::string query) {
    if (histogram.find(query) != histogram.end()) {
        histogram[query]++;
    } else {
        if (cur_window_size < max_window_size) {
            cur_window_size++;
        } else {
            int min = INT_MAX;
            string toBeEvicted;
            for ( auto it = histogram.begin(); it != histogram.end(); ++it ){
                if (it->second < min){
                    min = it->second;
                    toBeEvicted = it->first;
                }
            }
            mymap.erase (toBeEvicted);
        }
        histogram[query] = 1;
    }


    return histogram[query];
}

Uint32 WLfuHistogram::Estimate(std::string query) {
    if (histogram.find(query) != histogram.end())
        return histogram[query];
    else
        return -1;
}
