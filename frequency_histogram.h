//
// Created by yfy on 5/13/17.
//

#ifndef CENG778_PROJECT_FREQUENCY_HISTOGRAM_H
#define CENG778_PROJECT_FREQUENCY_HISTOGRAM_H

#include "types.h"
#include <string>

class FrequencyHistogram {
public:
    virtual Uint32 Add(std::string) = 0;
    virtual Uint32 Estimate(std::string) = 0;
};


#endif //CENG778_PROJECT_FREQUENCY_HISTOGRAM_H
