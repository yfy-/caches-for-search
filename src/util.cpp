//
// Created by yfy on 5/13/17.
//

#include <sstream>
#include <vector>
#include <unordered_map>
#include "util.h"

namespace utl {

    // Splits a string into a string array with a given delimiter

    std::vector<std::string> split(std::string s, char c) {
        std::string buff{""};
        std::vector<std::string> v;

        for (auto n:s) {
            if (n != c) buff += n;
            else if (n == c && buff != "") {
                v.push_back(buff);
                buff = "";
            }
        }

        if (buff != "") v.push_back(buff);
        return v;
    }

    std::uint64_t unique_size(const std::vector<std::string> &vector) {
        std::unordered_map<std::string, int> map;

        for (auto v: vector) {
            map[v] = 0;
        }

        return (std::uint64_t) map.size();
    }
}