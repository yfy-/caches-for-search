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
    std::unordered_map<std::string, Uint32> histogram;

public:
    Uint32 Add(std::string) override;

    Uint32 Estimate(std::string) override;
};


#endif //CENG778_PROJECT_PURE_LFU_H
