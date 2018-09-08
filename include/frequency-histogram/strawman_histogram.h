// Copyright 2017 folly

#ifndef CENG778_PROJECT_INCLUDE_FREQUENCY_HISTOGRAM_STRAWMAN_HISTOGRAM_H
#define CENG778_PROJECT_INCLUDE_FREQUENCY_HISTOGRAM_STRAWMAN_HISTOGRAM_H

#include <deque>
#include <string>
#include <vector>
#include "frequency-histogram/frequency_histogram.h"
#include "spectral_bloom_filter.h"

template <std::uint32_t counter_size_, std::uint32_t n_segments_,
  std::uint32_t bit_width_>
class StrawmanHistogram : public FrequencyHistogram {
 private:
  typedef SpectralBloomFilter<counter_size_ / n_segments_, bit_width_> Sbf;

  Sbf* current_;
  std::deque<Sbf*> fifo_;
  std::uint32_t segment_counter_;
  std::uint32_t n_hash_;
  std::uint32_t window_size_;

  std::vector<std::uint32_t> Hash(const std::string &kKey) const {
    std::uint32_t fnv = Fnv1a(kKey);
    std::uint64_t m_hash[2];
    MurmurHash3_x64_128(kKey.c_str(), (uint32_t) kKey.length(), 42, m_hash);
    auto murmur = static_cast<std::uint32_t >(m_hash[1] & 0x0000000FFFFFFFFULL);

    constexpr std::uint32_t counter_per_seg = counter_size_ / n_segments_;
    std::vector<std::uint32_t> hashed_indices = { fnv % counter_per_seg,
                                                  murmur % counter_per_seg};

    for (int i = 1; i <= n_hash_ - 2; ++i) {
      hashed_indices.push_back((fnv + i * murmur) % counter_per_seg);
    }

    return hashed_indices;
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
  StrawmanHistogram(std::uint32_t n_hash, std::uint32_t w): window_size_{w} {
    n_hash_ = n_hash < 2 ? 2 : n_hash;

    for (int i = 0; i < n_segments_ - 1; ++i) {
      fifo_.push_back(new Sbf(n_hash));
    }

    current_ = new Sbf(n_hash);
    segment_counter_ = 0;
  }

  uint32_t Add(const std::string &kQuery) override {
    std::vector<std::uint32_t> indices = Hash(kQuery);
    current_->Add(indices);
    segment_counter_++;

    std::uint32_t freq = Estimate(indices);

    if (segment_counter_ == window_size_ / n_segments_) {
      fifo_.push_back(current_);
      current_ = fifo_.front();
      fifo_.pop_front();
      current_->ResetCounters();
      segment_counter_ = 0;
    }
    return freq;
  }

  uint32_t Estimate(const std::string &kQuery) const override {
    std::vector<std::uint32_t> indices = Hash(kQuery);
    return Estimate(indices);
  }

  virtual ~StrawmanHistogram() {
    // Why do we need typename keyword?

    typename std::deque<Sbf*>::iterator it = fifo_.begin();
    while (it != fifo_.end()) {
      delete *it++;
    }

    delete current_;
  }

  std::string ToString() const override {
    std::string result = current_->ToString() + " -> ";

    typename std::deque<Sbf*>::const_iterator it = fifo_.begin();
    while (it != fifo_.end()) {
      result += (*it)->ToString() + " -> ";
      it++;
    }

    return result.substr(0, result.size() - 4);
  }
};

#endif  // CENG778_PROJECT_INCLUDE_FREQUENCY_HISTOGRAM_STRAWMAN_HISTOGRAM_H
