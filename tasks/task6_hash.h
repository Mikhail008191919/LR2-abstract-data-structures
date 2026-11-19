#pragma once

#include "../structures/singly_list.h"
#include <cstddef>
#include <cstdint>
#include <vector>

// Метод свёртки используется в обоих вариантах хеш-таблиц.
uint64_t magnitude(int64_t number);
uint64_t fold(int64_t number);

class ChainTable {
private:
    SinglyList* buckets;
    int capacity;

public:
    size_t comparisons = 0;
    explicit ChainTable(int size);
    ~ChainTable();
    ChainTable(const ChainTable&) = delete;
    ChainTable& operator=(const ChainTable&) = delete;

    bool has(int key) const;
    void add(int key);
};

class Cuckoo {
private:
    struct Cell {
        int64_t key = 0;
        bool used = false;
    };

    std::vector<Cell> first;
    std::vector<Cell> second;
    std::vector<int64_t> keys;

    size_t hash1(int64_t key) const;
    size_t hash2(int64_t key) const;
    bool place(int64_t key);
    void rebuild();

public:
    size_t displacements = 0;
    size_t rebuilds = 0;
    explicit Cuckoo(size_t capacity);

    bool has(int64_t key) const;
    void add(int64_t key);
};
