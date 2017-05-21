//
// Created by yfy on 5/13/17.
//

#ifndef CENG778_PROJECT_FREQUENCY_HISTOGRAM_H
#define CENG778_PROJECT_FREQUENCY_HISTOGRAM_H

#include <string>

class FrequencyHistogram {
public:
    virtual std::uint64_t Add(const std::string&) = 0;
    virtual std::uint64_t Estimate(const std::string&) const = 0;
};


#endif //CENG778_PROJECT_FREQUENCY_HISTOGRAM_H
