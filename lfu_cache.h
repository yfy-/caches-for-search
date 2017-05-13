//
// Created by yfy on 5/13/17.
//

#ifndef CENG778_PROJECT_LFU_CACHE_H
#define CENG778_PROJECT_LFU_CACHE_H

#include "types.h"
#include "frequency_histogram.h"
#include <string>
#include <unordered_map>

class LfuCache {

private:

    struct Node {
        std::string data;
        Node* next;
        Node(std::string d) : data{d} {}
    };

    Uint32 size_;
    Uint32 count_;

    Node* head_;
    std::unordered_map<std::string, Node*> cache_;
    FrequencyHistogram* freq_hist_;

    void FindPlace(Node*, Uint32);

public:
    inline Uint32 Size() const { return size_; }
    inline Uint32 Count() const { return count_; }
    bool IsExist(std::string);
    std::string ToString();

    LfuCache(Uint32, FrequencyHistogram*);

    virtual ~LfuCache();
};


#endif //CENG778_PROJECT_LFU_CACHE_H
