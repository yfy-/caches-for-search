//
// Created by yfy on 5/13/17.
//

#ifndef CENG778_PROJECT_UTIL_H
#define CENG778_PROJECT_UTIL_H

#include <string>
#include <vector>

namespace utl {
    std::vector<std::string> split(std::string, char);

    std::uint64_t unique_size(const std::vector<std::string> &);
}

#endif //CENG778_PROJECT_UTIL_H
