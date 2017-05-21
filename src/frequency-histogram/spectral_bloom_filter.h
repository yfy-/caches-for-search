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
                val += pow(2, bit_width_ - i - 1);
            }
        }

        return val;
    }

    void Add(std::uint64_t index) {

        std::uint64_t current = index + bit_width_;

        while (current > index) {
            counters_.flip(current - 1);

            if (counters_[current - 1] == 1)
                return;
            current--;
        }

        if (current == index)
            std::cout << "Overflow detected!" << std::endl;
    }

    void RightShift(std::uint64_t index) {

        std::uint64_t counter = 0;

        int spare = counters_[index];
        while (counter < bit_width_ - 1) {
            int temp = counters_[index + counter + 1];

            counters_[index + counter + 1] = spare;

            spare = temp;
            counter++;
        }

        counters_[index] = 0;
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
        std::uint64_t current = 0;

        while (current < window_size_) {
            RightShift(current * bit_width_);
            current++;
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
