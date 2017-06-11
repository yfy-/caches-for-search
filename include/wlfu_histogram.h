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
    std::unordered_map<std::string, std::uint64_t> histogram;
    std::uint64_t max_window_size;
    std::uint64_t cur_window_size;

public:
    WLfuHistogram(std::uint64_t m, std::uint64_t c=0) : max_window_size(m), cur_window_size(c) {}

    std::uint64_t Add(const std::string&) override;

    std::uint64_t Estimate(const std::string&) const override;
};


#endif //CENG778_PROJECT_PURE_LFU_H
