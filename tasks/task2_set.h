#pragma once

#include "../structures/singly_list.h"
#include <vector>

class IntSet {
private:
    SinglyList* buckets;
    int bucketCount;
    int elementCount;
    int bucketIndex(int value) const;
    void grow();

public:
    IntSet();
    ~IntSet();
    IntSet(const IntSet&) = delete;
    IntSet& operator=(const IntSet&) = delete;

    bool add(int value);
    bool del(int value);
    bool has(int value) const;
    int size() const;
    std::vector<int> values() const;
    void print() const;
};

bool fileQuery(int argc, char* argv[]);
