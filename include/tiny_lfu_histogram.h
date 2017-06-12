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

template <std::uint64_t window_size_, std::uint64_t bit_width_> class TinyLfuHistogram : public FrequencyHistogram {

private:
    std::bitset<sizeof(window_size_) * 8 -__builtin_clzl(window_size_)> window_counter_;
    SpectralBloomFilter<window_size_ / 2, bit_width_>* histogram_;
    SpectralBloomFilter<window_size_, 1>* doorkeeper_;

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

        std::uint64_t hist_count = 0;

        if (doorkeeper_->Estimate(kQuery) == 0) {
            doorkeeper_->Add(kQuery);
            hist_count = histogram_->Estimate(kQuery);
        } else {
            hist_count = histogram_->Add(kQuery);
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
        return doorkeeper_->Estimate(kQuery) + histogram_->Estimate(kQuery);
    }

    virtual ~TinyLfuHistogram() {
        delete histogram_;
        delete doorkeeper_;
    }
};


#endif //CENG778_PROJECT_TINY_LFU_HISTOGRAM_H
