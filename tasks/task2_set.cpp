#include "task2_set.h"
#include "../common.h"
#include "../tasks.h"
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>

using namespace std;

IntSet::IntSet() : buckets(new SinglyList[101]), bucketCount(101), elementCount(0) {}
IntSet::~IntSet() { delete[] buckets; }

int IntSet::bucketIndex(int value) const {
    long long index = static_cast<long long>(value) % bucketCount;
    if (index < 0) index += bucketCount;
    return static_cast<int>(index);
}

bool IntSet::has(int value) const {
    return buckets[bucketIndex(value)].contains(value);
}

bool IntSet::add(int value) {
    if (has(value)) return false;
    buckets[bucketIndex(value)].pushFront(value);
    ++elementCount;
    if (elementCount > bucketCount * 2) grow();
    return true;
}

bool IntSet::del(int value) {
    if (!buckets[bucketIndex(value)].removeValue(value)) return false;
    --elementCount;
    return true;
}

int IntSet::size() const { return elementCount; }

void IntSet::grow() {
    int newCount = bucketCount * 2 + 1;
    SinglyList* newBuckets = new SinglyList[newCount];
    for (int i = 0; i < bucketCount; ++i) {
        // forEach проходит по списку за один проход (O(k)).
        buckets[i].forEach([&](int value) {
            long long index = static_cast<long long>(value) % newCount;
            if (index < 0) index += newCount;
            newBuckets[index].pushFront(value);
        });
    }
    delete[] buckets;
    buckets = newBuckets;
    bucketCount = newCount;
}

vector<int> IntSet::values() const {
    vector<int> result;
    result.reserve(elementCount);
    for (int i = 0; i < bucketCount; ++i)
        buckets[i].forEach([&](int value) { result.push_back(value); });
    sort(result.begin(), result.end());
    return result;
}

void IntSet::print() const {
    vector<int> items = values();
    cout << '{';
    for (size_t i = 0; i < items.size(); ++i) {
        if (i) cout << ", ";
        cout << items[i];
    }
    cout << "}\n";
}

static bool readInt(const string& text, int& result) {
    int64_t number;
    if (!parseInt64(text, number)) return false;
    if (number < numeric_limits<int>::min() || number > numeric_limits<int>::max())
        return false;
    result = static_cast<int>(number);
    return true;
}

void runSetADT() {
    IntSet values;
    cout << "SETADD x, SETDEL x, SET_AT x, PRINT, EXIT\n";
    while (true) {
        cout << "SET> ";
        string line = readLine();
        if (!cin) return;
        vector<string> cmd = words(line);
        if (cmd.empty()) continue;
        string action = upper(cmd[0]);
        if (action == "EXIT") return;
        if (action == "PRINT") { values.print(); continue; }
        int key;
        if (cmd.size() != 2 || !readInt(cmd[1], key)) {
            cout << "Нужно целое число\n";
            continue;
        }
        if (action == "SETADD") cout << (values.add(key) ? "TRUE" : "FALSE") << '\n';
        else if (action == "SETDEL") cout << (values.del(key) ? "TRUE" : "FALSE") << '\n';
        else if (action == "SET_AT") cout << (values.has(key) ? "TRUE" : "FALSE") << '\n';
        else cout << "Неизвестная команда\n";
    }
}

bool fileQuery(int argc, char* argv[]) {
    if (argc != 5 || string(argv[1]) != "--file" || string(argv[3]) != "--query")
        return false;

    vector<string> cmd = words(argv[4]);
    int key;
    if (cmd.size() != 2 || !readInt(cmd[1], key)) {
        cerr << "Формат: SETADD/SETDEL/SET_AT целое_число\n";
        return true;
    }

    IntSet values;
    ifstream input(argv[2]);
    int saved;
    while (input >> saved) values.add(saved);

    string action = upper(cmd[0]);
    bool changed = false;
    if (action == "SETADD") {
        changed = values.add(key);
        cout << (changed ? "TRUE" : "FALSE") << '\n';
    } else if (action == "SETDEL") {
        changed = values.del(key);
        cout << (changed ? "TRUE" : "FALSE") << '\n';
    } else if (action == "SET_AT") {
        cout << (values.has(key) ? "TRUE" : "FALSE") << '\n';
    } else {
        cerr << "Неизвестная команда\n";
        return true;
    }

    if (changed) {
        ofstream output(argv[2]);
        if (!output) {
            cerr << "Не удалось записать файл\n";
        } else {
            for (int value : values.values()) output << value << '\n';
        }
    }
    return true;
}
