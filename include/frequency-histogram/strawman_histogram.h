// Copyright 2017 folly

#ifndef CENG778_PROJECT_INCLUDE_FREQUENCY_HISTOGRAM_STRAWMAN_HISTOGRAM_H
#define CENG778_PROJECT_INCLUDE_FREQUENCY_HISTOGRAM_STRAWMAN_HISTOGRAM_H

#include <deque>
#include <string>
#include <vector>
#include "frequency-histogram/frequency_histogram.h"
#include "spectral_bloom_filter.h"

template <std::uint32_t window_size_, std::uint32_t n_segments_,
  std::uint32_t bit_width_>
class StrawmanHistogram : public FrequencyHistogram {
 private:
  typedef SpectralBloomFilter<window_size_ / n_segments_, bit_width_> Sbf;

  Sbf* current_;
  std::deque<Sbf*> fifo_;
  std::bitset<sizeof(n_segments_) * 8 -__builtin_clz(n_segments_)>
      segment_counter_;

  void IncrementSegmentCounter() {
    for (int i = 0; i < segment_counter_.size(); ++i) {
      segment_counter_.flip(i);

      if (segment_counter_[i] == 1)
        return;
    }
  }

  std::vector<std::uint32_t> Hash(const std::string &kKey) const {
    std::uint32_t fnv = Fnv1a(kKey);
    std::uint64_t m_hash[2];

    MurmurHash3_x64_128(kKey.c_str(), (uint32_t) kKey.length(), 42, m_hash);
    std::uint32_t murmur = static_cast<std::uint32_t >(m_hash[1] & 0x00000000FFFFFFFFULL);
    std::uint32_t combined = fnv + murmur;

    return std::vector<std::uint32_t> {fnv % (window_size_ / n_segments_),
          murmur % (window_size_ / n_segments_),
          combined % (window_size_ / n_segments_)};
  }

  uint32_t Estimate(const std::vector<std::uint32_t> indices) const {
    std::uint32_t freq = 0;

    // Why do we need typename keyword?

    typename std::deque<Sbf*>::const_iterator it = fifo_.begin();
    while (it != fifo_.end()) {
      freq += (*it)->Estimate(indices);
      it++;
    }

    return freq + current_->Estimate(indices);
  }

 public:
  StrawmanHistogram() {
    for (int i = 0; i < n_segments_ - 1; ++i) {
      fifo_.push_back(new Sbf());
    }

    current_ = new Sbf();
  }

  uint32_t Add(const std::string &kQuery) override {
    std::vector<std::uint32_t> indices = Hash(kQuery);
    current_->Add(indices);

    std::uint32_t freq = Estimate(indices);

    if (segment_counter_.to_ulong() == window_size_ / n_segments_) {
      fifo_.push_back(current_);
      current_ = fifo_.front();
      fifo_.pop_front();
      current_->ResetCounters();
      segment_counter_.reset();
    } else {
      IncrementSegmentCounter();
    }

    return freq;
  }

  uint32_t Estimate(const std::string &kQuery) const override {
    std::vector<std::uint32_t> indices = Hash(kQuery);
    std::uint32_t freq = 0;

    // Why do we need typename keyword?

    typename std::deque<Sbf*>::const_iterator it = fifo_.begin();
    while (it != fifo_.end()) {
      freq += (*it)->Estimate(indices);
      it++;
    }

    return freq + current_->Estimate(indices);
  }

  virtual ~StrawmanHistogram() {
    // Why do we need typename keyword?

    typename std::deque<Sbf*>::iterator it = fifo_.begin();
    while (it != fifo_.end()) {
      delete *it++;
    }

    delete current_;
  }
};

#endif  // CENG778_PROJECT_INCLUDE_FREQUENCY_HISTOGRAM_STRAWMAN_HISTOGRAM_H
