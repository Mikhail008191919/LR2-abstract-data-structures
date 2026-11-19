#include "task6_hash.h"
#include "../tasks.h"
#include "../common.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <utility>

using namespace std;

uint64_t magnitude(int64_t number) {
    if (number >= 0) return static_cast<uint64_t>(number);
    return static_cast<uint64_t>(-(number + 1)) + 1;
}

uint64_t fold(int64_t number) {
    uint64_t rest = magnitude(number);
    uint64_t sum = 0;
    do {
        sum += rest % 1000;
        rest /= 1000;
    } while (rest > 0);
    return sum;
}

ChainTable::ChainTable(int size)
    : buckets(new SinglyList[size]), capacity(size) {}

ChainTable::~ChainTable() { delete[] buckets; }

bool ChainTable::has(int key) const {
    int index = static_cast<int>(fold(key) % capacity);
    return buckets[index].contains(key);
}

void ChainTable::add(int key) {
    int index = static_cast<int>(fold(key) % capacity);
    bool found = false;
    // Считаем каждое сравнение с ключом в цепочке, включая дубликаты.
    buckets[index].forEach([&](int current) {
        ++comparisons;
        if (current == key) found = true;
    });
    if (!found) buckets[index].pushFront(key);
}

Cuckoo::Cuckoo(size_t capacity)
    : first(max<size_t>(3, capacity)), second(max<size_t>(3, capacity)) {}

size_t Cuckoo::hash1(int64_t key) const {
    return fold(key) % first.size();
}

size_t Cuckoo::hash2(int64_t key) const {
    uint64_t number = static_cast<uint64_t>(key);
    return (number * 1315423911ULL + (number >> 7) + 7) % second.size();
}

bool Cuckoo::has(int64_t key) const {
    const Cell& a = first[hash1(key)];
    const Cell& b = second[hash2(key)];
    return (a.used && a.key == key) || (b.used && b.key == key);
}

bool Cuckoo::place(int64_t key) {
    bool useFirst = true;
    for (size_t attempt = 0; attempt < 2 * first.size(); ++attempt) {
        Cell& cell = useFirst ? first[hash1(key)] : second[hash2(key)];
        if (!cell.used) {
            cell.key = key;
            cell.used = true;
            return true;
        }
        if (cell.key == key) return true;
        swap(key, cell.key);
        ++displacements;
        useFirst = !useFirst;
    }
    return false;
}

void Cuckoo::rebuild() {
    size_t nextSize = first.size() * 2 + 1;
    while (true) {
        if (nextSize > 2'000'001)
            throw runtime_error("Не удалось перестроить хеш-таблицу");
        ++rebuilds;
        first.assign(nextSize, {});
        second.assign(nextSize, {});
        bool success = true;
        for (int64_t key : keys) {
            if (!place(key)) {
                success = false;
                break;
            }
        }
        if (success) return;
        nextSize = nextSize * 2 + 1;
    }
}

void Cuckoo::add(int64_t key) {
    if (has(key)) return;
    keys.push_back(key);
    if (!place(key)) rebuild();
}

void runHashTable() {
    cout << "Размер таблицы: ";
    int64_t size = readNum();
    if (size < 3 || size > 100000) {
        cout << "Неверный размер\n";
        return;
    }

    cout << "Ключи через пробел (32-битные целые): ";
    vector<int> keys;
    for (const string& text : words(readLine())) {
        int64_t key;
        if (parseInt64(text, key) && key >= numeric_limits<int>::min() &&
            key <= numeric_limits<int>::max())
            keys.push_back(static_cast<int>(key));
    }

    ChainTable chain(static_cast<int>(size));
    Cuckoo cuckoo(static_cast<size_t>(size));
    auto started = chrono::steady_clock::now();
    for (int key : keys) chain.add(key);
    auto chainTime = chrono::duration_cast<chrono::nanoseconds>(
        chrono::steady_clock::now() - started).count();

    try {
        started = chrono::steady_clock::now();
        for (int key : keys) cuckoo.add(key);
    } catch (const exception& error) {
        cout << "Ошибка хеширования: " << error.what() << '\n';
        return;
    }
    auto cuckooTime = chrono::duration_cast<chrono::nanoseconds>(
        chrono::steady_clock::now() - started).count();

    cout << "Цепочки: сравнений " << chain.comparisons
         << ", время " << chainTime << " нс\n";
    cout << "Кукушка: вытеснений " << cuckoo.displacements
         << ", перестроений " << cuckoo.rebuilds
         << ", время " << cuckooTime << " нс\n";
    for (int key : keys) {
        if (!chain.has(key) || !cuckoo.has(key))
            cout << "ОШИБКА поиска: " << key << '\n';
    }
}
