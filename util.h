//
// Created by yfy on 5/13/17.
//

#ifndef CENG778_PROJECT_UTIL_H
#define CENG778_PROJECT_UTIL_H

#include <string>
#include <vector>
#include "types.h"

namespace utl {
    std::vector<std::string> split(std::string, char);

    Uint32 unique_size(std::vector<std::string>);
}

#endif //CENG778_PROJECT_UTIL_H
