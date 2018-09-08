// Copyright 2017 yfy
#ifndef CENG778_PROJECT_INCLUDE_FREQUENCY_HISTOGRAM_TINY_LFU_HISTOGRAM_H
#define CENG778_PROJECT_INCLUDE_FREQUENCY_HISTOGRAM_TINY_LFU_HISTOGRAM_H

#include <bitset>
#include <string>
#include "frequency-histogram/frequency_histogram.h"
#include "spectral_bloom_filter.h"

template <std::uint32_t counter_size_, std::uint32_t bit_width_>
class TinyLfuHistogram : public FrequencyHistogram {
 private:
  std::uint32_t window_counter_;
  std::uint32_t window_size_;
  SpectralBloomFilter<counter_size_ / 2, bit_width_>* histogram_;
  SpectralBloomFilter<counter_size_, 1>* doorkeeper_;

 public:
    TinyLfuHistogram(std::uint32_t n_hash, std::uint32_t w) : window_size_(w) {
    // Assuming that half of the queries are not repeating
    histogram_ = new SpectralBloomFilter<counter_size_ / 2, bit_width_>(n_hash);
    doorkeeper_ = new SpectralBloomFilter<counter_size_, 1>(n_hash);
    window_counter_ = 0;
  }

  uint32_t Add(const std::string &kQuery) override {
    std::uint32_t hist_count = 0;
    window_counter_++;

    if (doorkeeper_->Estimate(kQuery) == 0) {
      doorkeeper_->Add(kQuery);
      hist_count = histogram_->Estimate(kQuery);
    } else {
      hist_count = histogram_->Add(kQuery);
    }

    if (window_counter_ == window_size_) {
      window_counter_ >>= 1;
      doorkeeper_->ResetCounters();
      histogram_->RightShiftCounters();
    }
    return hist_count + 1;
  }

  uint32_t Estimate(const std::string &kQuery) const override {
    return doorkeeper_->Estimate(kQuery) + histogram_->Estimate(kQuery);
  }

  virtual ~TinyLfuHistogram() {
    delete histogram_;
    delete doorkeeper_;
  }

  std::string ToString() const override {
    return "FullCounters -> " + histogram_->ToString() + "\nDoorKeeper -> "
        + doorkeeper_->ToString();
  }
};


#endif //CENG778_PROJECT_INCLUDE_FREQUENCY_HISTOGRAM_TINY_LFU_HISTOGRAM_H
