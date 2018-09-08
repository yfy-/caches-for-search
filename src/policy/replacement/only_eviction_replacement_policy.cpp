// Created by yfy on 28/03/2018

#include "policy/replacement/only_eviction_replacement_policy.h"
#include "policy/eviction/eviction_policy.h"

OnlyEvictionReplacementPolicy::OnlyEvictionReplacementPolicy(
    EvictionPolicy* e, const std::string& n) : ReplacementPolicy(n),
                                               eviction_{e} {}

bool OnlyEvictionReplacementPolicy::IsExist(const std::string& query) {
  bool exist = eviction_->IsExist(query);

  if (exist) {
    eviction_->Update(query);
  } else {
    if (eviction_->IsEmpty())
      eviction_->Add(query);
    else
      eviction_->ReplaceVictim(query);
  }

  return exist;
}

OnlyEvictionReplacementPolicy::~OnlyEvictionReplacementPolicy() {}
