// Created by yfy on 21/04/2018

#ifndef CENG778PROJECT_TOPRETRIEVER_H
#define CENG778PROJECT_TOPRETRIEVER_H

#include <functional>
#include <vector>

// A special case of a heap to retrieve top-k elements
template<class T>
class TopRetriever {

 public:
  TopRetriever(std::uint32_t, std::function<bool(const T&, const T&)>);
  ~TopRetriever();

  void Push(const T&);
  void Sort(T*);

  inline bool IsFull() { return count_ == size_; }
  inline std::uint32_t GetCount() { return count_; }

 private:
  std::uint32_t size_;
  std::uint32_t count_;
  std::function<bool(const T&, const T&)> comp_;
  T* elems_;

  inline std::uint32_t Left(std::uint32_t i) { return 2 * i + 1; }
  inline std::uint32_t Right(std::uint32_t i) { return 2 * i + 2; }
  std::int32_t Parent(std::uint32_t);
  void Swim(std::uint32_t);
  void Sink(std::uint32_t);
  void Swap(std::uint32_t, std::uint32_t);
};

template<class T>
TopRetriever<T>::TopRetriever(std::uint32_t s,
                           std::function<bool(const T&, const T&)> c) {
  size_ = s;
  count_ = 0;
  comp_ = c;
  elems_ = new T[size_];
}

template<class T>
TopRetriever<T>::~TopRetriever() {
  delete elems_;
}

template<class T>
void TopRetriever<T>::Push(const T& e) {
  if (!IsFull()) {
    elems_[count_] = e;
    count_++;
    Swim(count_ - 1);
  } else {
    if (comp_(e, elems_[0])) {
      elems_[0] = e;
      Sink(0);
    }
  }
}

template<class T>
void TopRetriever<T>::Sort(T* sorted) {
  std::uint32_t c = count_;

  for (std::uint32_t i = 0; i < c; ++i) {
    sorted[c - (i + 1)] = elems_[0];
    Swap(0, count_ - 1);
    count_--;
    Sink(0);
  }
}

template<class T>
std::int32_t TopRetriever<T>::Parent(std::uint32_t i) {
  if (i == 0)
    return -1;

  std::int32_t p = i / 2;
  return i % 2 == 1 ? p : p - 1;
}

template<class T>
void TopRetriever<T>::Swim(std::uint32_t i) {
  std::int32_t p = Parent(i);

  if (p == -1)
    return;

  if (!comp_(elems_[i], elems_[p])) {
      Swap(i, p);
      Swim(p);
    }
}

template<class T>
void TopRetriever<T>::Sink(std::uint32_t i) {
  std::uint32_t p = i;

  std::uint32_t l = Left(i);
  if (l < count_ && comp_(elems_[p], elems_[l]))
    p = l;

  std::uint32_t r = Right(i);
  if (r < count_ && comp_(elems_[p], elems_[r]))
    p = r;

  if (p != i) {
    Swap(p, i);
    Sink(p);
  }
}

template<class T>
void TopRetriever<T>::Swap(std::uint32_t l, std::uint32_t r) {
  T temp = elems_[l];
  elems_[l] = elems_[r];
  elems_[r] = temp;
}

#endif  // CENG778PROJECT_TOPRETRIEVER_H
