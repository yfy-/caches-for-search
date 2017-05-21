// Created by yfy on 5/13/17.
//
#ifndef CENG778_PROJECT_PURE_LFU_H
#define CENG778_PROJECT_PURE_LFU_H

#include <unordered_map>
#include <string>
#include "limits.h"
#include "frequency_histogram.h"

class WLfuHistogram : public FrequencyHistogram {

private:
    std::unordered_map<std::string, Uint32> histogram;
    Uint32 max_window_size;
    Uint32 cur_window_size;

public:
    WLfuHistogram(Uint32 m, Uint32 c=0) : max_window_size(m), cur_window_size(c) {}

    Uint32 Add(std::string) override;

    Uint32 Estimate(std::string) override;
};


#endif //CENG778_PROJECT_PURE_LFU_H
