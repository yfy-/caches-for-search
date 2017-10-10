// Copyright 2017 folly

#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "util.h"


namespace utl {

std::vector<std::string> split(std::string s, char c) {
  std::string buff{""};
  std::vector<std::string> v;

  for (auto n : s) {
    if (n != c) {
      buff += n;
    } else if (n == c && buff != "") {
      v.push_back(buff);
      buff = "";
    }
  }

  if (buff != "") v.push_back(buff);
  return v;
}

std::uint32_t unique_size(const std::vector<std::string> &vector) {
  std::unordered_map<std::string, int> map;

  for (auto v : vector) {
    map[v] = 0;
  }

  return (std::uint32_t) map.size();
}

}  // namespace utl

