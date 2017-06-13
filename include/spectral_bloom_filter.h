//
// Created by yfy on 5/15/17.
//

#ifndef CENG778_PROJECT_SPECTRAL_BLOOM_FILTER_H
#define CENG778_PROJECT_SPECTRAL_BLOOM_FILTER_H

#include <bitset>
#include <vector>
#include <cmath>
#include "murmur3.h"
#include "fnv1a.h"

template <std::uint64_t window_size_, std::uint64_t bit_width_> class SpectralBloomFilter {

private:

    std::bitset<window_size_ * bit_width_> counters_;

    std::uint64_t Read(std::uint64_t index) {
        std::uint64_t val = 0;

        for (int i = 0; i < bit_width_; ++i) {
            if (counters_[index + i] == 1) {
                val += pow(2, i);
            }
        }

        return val;
    }

    void Add(std::uint64_t index) {

        for (int i = 0; i < bit_width_; ++i) {
            counters_.flip(index + i);
            if (counters_[index + i] == 1){
                return;
            }
        }

        for (int i = 0; i < bit_width_; ++i) {
            counters_.flip(index + i);
        }
    }

    void RightShift(std::uint64_t index) {
        for (int i = 0; i < bit_width_ - 1; ++i) {
            counters_[index + i] = counters_[index + i + 1];
        }

        counters_[index + bit_width_ - 1] = 0;
    }

    std::vector<std::uint64_t> Hash(const std::string &kKey) const {
        std::uint64_t fnv = Fnv1a(kKey);
        std::uint64_t m_hash[2];

        MurmurHash3_x64_128(kKey.c_str(), (uint32_t) kKey.length(), 42, m_hash);

        std::uint64_t combined = fnv + m_hash[1];

        return std::vector<std::uint64_t> {fnv % window_size_, m_hash[1] % window_size_,
                                           combined % window_size_};
    }

public:

    std::uint64_t Add(const std::string& kQuery) {

        std::vector<std::uint64_t> indices { Hash(kQuery) };
        return Add(indices);
    }

    std::uint64_t Add(const std::vector<std::uint64_t> & kIndices) {
        std::uint64_t min = Read(kIndices[0] * bit_width_);
        std::vector<std::uint64_t> to_be_added {kIndices[0]};

        for (int i = 1; i < kIndices.size(); ++i) {
            std::uint64_t current = Read(kIndices[i] * bit_width_);

            if (current < min) {
                min = current;
                to_be_added.clear();
            }
            to_be_added.push_back(kIndices[i]);
        }

        for (auto t: to_be_added) {
            Add(t * bit_width_);
        }

        return min + 1;
    }

    std::uint64_t Estimate(const std::string& kQuery) {

        std::vector<std::uint64_t> indices { Hash(kQuery) };
        std::uint64_t min = Read(indices[0] * bit_width_);

        for (int i = 1; i < indices.size(); ++i) {
            std::uint64_t current = Read(indices[i] * bit_width_);

            if (current < min) {
                min = current;
            }
        }

        return min;
    }

    std::uint64_t Estimate(const std::vector<std::uint64_t>& kIndices) {

        std::uint64_t min = Read(kIndices[0] * bit_width_);

        for (int i = 1; i < kIndices.size(); ++i) {
            std::uint64_t current = Read(kIndices[i] * bit_width_);

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
};

#endif //CENG778_PROJECT_SPECTRAL_BLOOM_FILTER_H
