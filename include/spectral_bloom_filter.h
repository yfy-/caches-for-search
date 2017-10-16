// Copyright 2017 folly

#ifndef CENG778_PROJECT_INCLUDE_SPECTRAL_BLOOM_FILTER_H
#define CENG778_PROJECT_INCLUDE_SPECTRAL_BLOOM_FILTER_H

#include <string>
#include <bitset>
#include <vector>
#include <cmath>
#include <set>
#include "frequency-histogram/frequency_histogram.h"
#include "hash/murmur3.h"
#include "hash/fnv1a.h"


template <std::uint32_t window_size_, std::uint32_t bit_width_>
class SpectralBloomFilter {
 private:
  std::bitset<window_size_ * bit_width_> counters_;
  std::uint32_t n_hash_;
  const std::uint32_t kMaxInt_;

  std::uint32_t Read(std::uint32_t index) const {
    std::uint32_t val = 0;

    for (int i = 0; i < bit_width_; ++i) {
      if (counters_[index + i] == 1)
        val += pow(2, i);
    }

    return val;
  }

  void Add(std::uint32_t index) {
    for (int i = 0; i < bit_width_; ++i) {
      counters_.flip(index + i);
      if (counters_[index + i] == 1) {
        return;
      }
    }

    for (int i = 0; i < bit_width_; ++i) {
      counters_.flip(index + i);
    }
  }

  void RightShift(std::uint32_t index) {
    for (int i = 0; i < bit_width_ - 1; ++i) {
      counters_[index + i] = counters_[index + i + 1];
    }
    counters_[index + bit_width_ - 1] = 0;
  }


  std::vector<std::uint32_t> Hash(const std::string &kKey) const {
    std::uint32_t fnv = Fnv1a(kKey);
    std::uint64_t m_hash[2];
    MurmurHash3_x64_128(kKey.c_str(), (uint32_t) kKey.length(), 42, m_hash);
    auto murmur = static_cast<std::uint32_t >(m_hash[1] & 0x0000000FFFFFFFFULL);

    std::vector<std::uint32_t> hashed_indices = { fnv % window_size_,
                                                  murmur % window_size_ };

    for (int i = 1; i <= n_hash_ - 2; ++i) {
      hashed_indices.push_back((fnv + i * murmur) % window_size_);
    }

    return hashed_indices;
  }

 public:

  explicit SpectralBloomFilter(uint32_t n_hash):
      kMaxInt_{ static_cast<std::uint32_t >(pow(2, bit_width_)) - 1 } {
    n_hash_ = n_hash < 2 ? 2 : n_hash;
  }

  std::uint32_t Add(const std::string& kQuery) {
    std::vector<std::uint32_t> indices { Hash(kQuery) };
    return Add(indices);
  }

  std::uint32_t Add(const std::vector<std::uint32_t> & kIndices) {
    std::set<std::uint32_t> unq_indices(kIndices.begin(), kIndices.end());
    std::vector<std::uint32_t> unq_indices_vec(unq_indices.begin(), unq_indices.end());

    std::uint32_t min = Read(unq_indices_vec[0] * bit_width_);
    std::vector<std::uint32_t> to_be_added { unq_indices_vec[0] };

    for (int i = 1; i < unq_indices_vec.size(); ++i) {
      std::uint32_t current = Read(unq_indices_vec[i] * bit_width_);

      if (current < min) {
        min = current;
        to_be_added.clear();
      }

      if (current == min)
        to_be_added.push_back(unq_indices_vec[i]);
    }

    for (auto t : to_be_added) {
      Add(t * bit_width_);
    }

    if (min == kMaxInt_)
      return min;

    return min + 1;
  }

  std::uint32_t Estimate(const std::string& kQuery) {
    std::vector<std::uint32_t> indices { Hash(kQuery) };
    return Estimate(indices);
  }

  std::uint32_t Estimate(const std::vector<std::uint32_t>& kIndices) {
    std::uint32_t min = Read(kIndices[0] * bit_width_);

    for (int i = 1; i < kIndices.size(); ++i) {
      std::uint32_t current = Read(kIndices[i] * bit_width_);

      if (current < min) {
        min = current;

      }
    }

    return min;
  }

  void RightShiftCounters() {
    for (int i = 0; i < window_size_; ++i) {
      RightShift(i * bit_width_);
    }
  }

  void ResetCounters() {
    counters_.reset();
  }

  std::string ToString() const {
    std::string result;

    for (int i = 0; i < window_size_; ++i) {
      result += std::to_string(Read(i * bit_width_)) + " ";
    }

    return result.substr(0, result.size() - 1);
  }
};

#endif  // CENG778_PROJECT_INCLUDE_SPECTRAL_BLOOM_FILTER_H
