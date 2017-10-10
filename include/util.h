// Copyright 2017 folly
#ifndef CENG778_PROJECT_INCLUDE_UTIL_H
#define CENG778_PROJECT_INCLUDE_UTIL_H

#include <string>
#include <vector>

namespace utl {
  std::vector<std::string> split(std::string, char);
  std::uint32_t unique_size(const std::vector<std::string> &);
}

#endif  // CENG778_PROJECT_INCLUDE_UTIL_H
