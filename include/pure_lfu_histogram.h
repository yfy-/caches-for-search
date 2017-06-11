//
// Created by yfy on 5/13/17.
//

#ifndef CENG778_PROJECT_PURE_LFU_H
#define CENG778_PROJECT_PURE_LFU_H

#include <unordered_map>
#include <string>
#include "frequency_histogram.h"

class PureLfuHistogram : public FrequencyHistogram {

private:
    std::unordered_map<std::string, std::uint64_t> histogram;

public:
    std::uint64_t Add(const std::string &) override;

    std::uint64_t Estimate(const std::string &) const override;
};


#endif //CENG778_PROJECT_PURE_LFU_H
