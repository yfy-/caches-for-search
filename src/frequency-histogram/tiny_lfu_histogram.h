//
// Created by yfy on 5/20/17.
//

#ifndef CENG778_PROJECT_TINY_LFU_HISTOGRAM_H
#define CENG778_PROJECT_TINY_LFU_HISTOGRAM_H


#include <cstdint>
#include <bitset>
#include <cmath>
#include "frequency_histogram.h"
#include "spectral_bloom_filter.h"
#include "../hash/fnv1a.h"
#include "../hash/murmur3.h"

template <std::uint64_t window_size_> class TinyLfuHistogram : public FrequencyHistogram {

private:
    std::bitset<(std::uint64_t)ceil(log2(window_size_))> window_counter_;
    SpectralBloomFilter* histogram_;
    SpectralBloomFilter* doorkeeper_;

    std::vector<std::uint64_t> Hash(const std::string &kKey) const {
        std::uint64_t fnv_index = Fnv1a(kKey) % window_size_;
        std::uint64_t m_hash[2];

        MurmurHash3_x64_128(kKey.c_str(), (uint32_t) kKey.length(), 42, m_hash);

        std::uint64_t murmur_index = m_hash[1] % window_size_;

        return std::vector<std::uint64_t> {fnv_index, murmur_index};
    }

    void IncrementWindowCounter() {
        std::uint64_t counter = (std::uint64_t) ceil(log2(window_size_));

        while (counter > 0) {
            window_counter_.flip(counter - 1);

            if (window_counter_[counter - 1] == 1)
                return;
            counter--;
        }

        if (counter == 0)
            std::cout << "Overflow in window counter!" << std::endl;
    }

public:

    TinyLfuHistogram(std::uint64_t cache_size) {
        std::uint64_t bit_width =
                (std::uint64_t) (floor(log2(ceil(window_size_/ (double) cache_size) - 1)) + 1);

//        Assuming that half of the queries are not repeating
        histogram_= new SpectralBloomFilter<window_size_ / 2, bit_width>();

        doorkeeper_ = new SpectralBloomFilter<window_size_, 1>();
    }

    uint64_t Add(const std::string &query) override {

        std::vector<std::uint64_t> indices = Hash(query);
        std::uint64_t hist_count = 0;

        if (doorkeeper_->Estimate(indices) == 0) {
            doorkeeper_->Add(indices);
            hist_count = histogram_->Estimate(indices);
        } else {
            hist_count = histogram_->Add(indices);
        }

        if (window_counter_.all()) {
            window_counter_ >>= 1;
            doorkeeper_->Reset();
            histogram_->Reset();
        } else {
            IncrementWindowCounter();
        }

        return hist_count + 1;
    }

    uint64_t Estimate(const std::string &kQuery) const override {
        std::vector<std::uint64_t> indices = Hash(kQuery);
        return doorkeeper_->Estimate(indices) + histogram_->Estimate(indices);
    }

    virtual ~TinyLfuHistogram() {
        delete histogram_;
        delete doorkeeper_;
    }
};


#endif //CENG778_PROJECT_TINY_LFU_HISTOGRAM_H
