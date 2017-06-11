//
// Created by yfy on 5/20/17.
//

#ifndef CENG778_PROJECT_TINY_LFU_HISTOGRAM_H
#define CENG778_PROJECT_TINY_LFU_HISTOGRAM_H

#include <cstdint>
#include <bitset>
#include <cmath>
#include <chrono>
#include <thread>
#include "frequency_histogram.h"
#include "spectral_bloom_filter.h"
#include "fnv1a.h"
#include "murmur3.h"

template <std::uint64_t window_size_, std::uint64_t bit_width_> class TinyLfuHistogram : public FrequencyHistogram {

private:
    std::bitset<sizeof(window_size_) * 8 -__builtin_clzl(window_size_)> window_counter_;
    SpectralBloomFilter<window_size_ / 2, bit_width_>* histogram_;
    SpectralBloomFilter<window_size_, 1>* doorkeeper_;

    std::vector<std::uint64_t> Hash(const std::string &kKey, std::uint64_t range) const {
        std::uint64_t fnv_index = Fnv1a(kKey) % range;
        std::uint64_t m_hash[2];

        MurmurHash3_x64_128(kKey.c_str(), (uint32_t) kKey.length(), 42, m_hash);

        std::uint64_t murmur_index = m_hash[1] % range;

        return std::vector<std::uint64_t> {fnv_index, murmur_index};
    }

    void IncrementWindowCounter() {
        for (int i = 0; i < window_counter_.size(); ++i) {
            window_counter_.flip(i);
            if (window_counter_[i] == 1) {
                return;
            }
        }

        std::cout << "Overflow in window counter" << std::endl;
    }

public:

    TinyLfuHistogram() {
        //Assuming that half of the queries are not repeating
        histogram_ = new SpectralBloomFilter<window_size_ / 2, bit_width_>();
        doorkeeper_ = new SpectralBloomFilter<window_size_, 1>();
    }

    uint64_t Add(const std::string &kQuery) override {
        std::vector<std::uint64_t> indices_dk = Hash(kQuery, window_size_);
        std::vector<std::uint64_t> indices_h = Hash(kQuery, window_size_ / 2);

        std::uint64_t hist_count = 0;

        if (doorkeeper_->Estimate(indices_dk) == 0) {
            doorkeeper_->Add(indices_dk);
            hist_count = histogram_->Estimate(indices_h);
        } else {
            hist_count = histogram_->Add(indices_h);
        }

        if (window_counter_.to_ulong() == window_size_) {
            window_counter_ >>= 1;
            doorkeeper_->Reset();
            histogram_->Reset();
        } else {
            IncrementWindowCounter();
        }

        return hist_count + 1;
    }

    uint64_t Estimate(const std::string &kQuery) const override {
        std::vector<std::uint64_t> indices_dk = Hash(kQuery, window_size_);
        std::vector<std::uint64_t> indices_h = Hash(kQuery, window_size_ / 2);
        return doorkeeper_->Estimate(indices_dk) + histogram_->Estimate(indices_h);
    }

    virtual ~TinyLfuHistogram() {
        delete histogram_;
        delete doorkeeper_;
    }
};


#endif //CENG778_PROJECT_TINY_LFU_HISTOGRAM_H
