//
// Created by yfy on 5/15/17.
//

#ifndef CENG778_PROJECT_SPECTRAL_BLOOM_FILTER_H
#define CENG778_PROJECT_SPECTRAL_BLOOM_FILTER_H

#include <bitset>
#include <vector>
#include <cmath>
#include <iostream>

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


public:

    std::uint64_t Add(const std::vector<std::uint64_t> &indices) {

        std::uint64_t min = Read(indices[0] * bit_width_);
        std::vector<std::uint64_t> to_be_added {indices[0]};

        for (int i = 1; i < indices.size(); ++i) {
            std::uint64_t current = Read(indices[i] * bit_width_);

            if (current < min) {
                min = current;
                to_be_added.clear();
                to_be_added.push_back(indices[i]);
            } else if (current == min) {
                to_be_added.push_back(indices[i]);
            }
        }

        for (auto t: to_be_added) {
            Add(t * bit_width_);
        }

        return min + 1;
    }

    std::uint64_t Estimate(const std::vector<std::uint64_t> &indices) {

        std::uint64_t min = Read(indices[0] * bit_width_);

        for (int i = 1; i < indices.size(); ++i) {
            std::uint64_t current = Read(indices[i] * bit_width_);

            if (current < min) {
                min = current;
            }
        }

        return min;
    }

    void Reset() {
        for (int i = 0; i < window_size_; ++i) {
            RightShift(i * bit_width_);
        }
    }

    std::string ToString() {
        std::string result = "";

        for (int i = 0; i < window_size_; ++i) {
            result += (std::to_string(Read(i * bit_width_)) + ", ");
        }

        return result;
    }
};

#endif //CENG778_PROJECT_SPECTRAL_BLOOM_FILTER_H
