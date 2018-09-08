// Created by yfy on 28/03/2018

#ifndef CENG778PROJECT_POLICY_EVICTION_EVICTIONPOLICY_H
#define CENG778PROJECT_POLICY_EVICTION_EVICTIONPOLICY_H

#include <cstdint>
#include <string>

class EvictionPolicy {
 public:

  virtual bool IsExist(const std::string&) const = 0;
  virtual void Update(const std::string&, std::uint32_t = 1) = 0;
  virtual bool IsEmpty() const = 0;
  virtual std::string Victim() const = 0;
  virtual std::string ReplaceVictim(const std::string&, std::uint32_t = 1) = 0;
  virtual void Add(const std::string&, std::uint32_t = 1) = 0;
  virtual std::string CacheToString() const = 0;

 protected:
  EvictionPolicy(std::uint32_t s): size_{s}, count_{0} {}
  std::uint32_t size_;
  std::uint32_t count_;
};

#endif // CENG778PROJECT_POLICY_EVICTION_EVICTIONPOLICY_H
