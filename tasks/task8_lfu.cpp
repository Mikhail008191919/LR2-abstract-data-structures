#include "../tasks.h"
#include "../common.h"
#include "../structures/array.h"
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

class LFUCache {
private:
    IntArray keys;
    IntArray values;
    IntArray frequencies;
    IntArray lastUsed;
    int capacity;
    int clock = 0;

    int find(int key) const {
        for (int i = 0; i < keys.length(); ++i) {
            int current;
            keys.get(i, current);
            if (current == key) return i;
        }
        return -1;
    }

    void removeLeastUsed() {
        int victim = 0;
        int minFrequency, oldestTime;
        frequencies.get(0, minFrequency);
        lastUsed.get(0, oldestTime);

        for (int i = 1; i < keys.length(); ++i) {
            int frequency, time;
            frequencies.get(i, frequency);
            lastUsed.get(i, time);
            if (frequency < minFrequency ||
                (frequency == minFrequency && time < oldestTime)) {
                victim = i;
                minFrequency = frequency;
                oldestTime = time;
            }
        }

        // Все четыре массива должны сохранить одинаковый порядок индексов.
        keys.remove(victim);
        values.remove(victim);
        frequencies.remove(victim);
        lastUsed.remove(victim);
    }

public:
    explicit LFUCache(int size) : capacity(size) {}

    int get(int key) {
        int position = find(key);
        if (position == -1) return -1;
        int value, frequency;
        values.get(position, value);
        frequencies.get(position, frequency);
        frequencies.set(position, frequency + 1);
        lastUsed.set(position, ++clock);
        return value;
    }

    void set(int key, int value) {
        if (capacity <= 0) return;
        int position = find(key);
        if (position != -1) {
            int frequency;
            frequencies.get(position, frequency);
            values.set(position, value);
            frequencies.set(position, frequency + 1);
            lastUsed.set(position, ++clock);
            return;
        }
        if (keys.length() == capacity) removeLeastUsed();
        keys.pushBack(key);
        values.pushBack(value);
        frequencies.pushBack(1);
        lastUsed.pushBack(++clock);
    }
};

void runLFUCache() {
    cout << "Ёмкость кэша: ";
    int64_t capacity = readNum();
    cout << "Количество команд: ";
    int64_t commands = readNum();
    if (capacity < 0 || capacity > 100000 || commands < 0 || commands > 100000) {
        cout << "Неверные параметры\n";
        return;
    }

    LFUCache cache(static_cast<int>(capacity));
    cout << "Команды: SET ключ значение / GET ключ\n";
    for (int64_t i = 0; i < commands;) {
        vector<string> cmd = words(readLine());
        if (!cin) break;
        if (cmd.empty()) continue;
        string name = upper(cmd[0]);
        int64_t key = 0, value = 0;
        bool valid = (name == "GET" && cmd.size() == 2) ||
                     (name == "SET" && cmd.size() == 3);
        if (valid) valid = parseInt64(cmd[1], key);
        if (valid && name == "SET") valid = parseInt64(cmd[2], value);
        if (!valid || key < numeric_limits<int>::min() ||
            key > numeric_limits<int>::max() ||
            value < numeric_limits<int>::min() ||
            value > numeric_limits<int>::max()) {
            cout << "Неверная команда\n";
            continue;
        }
        if (name == "GET") cout << cache.get(static_cast<int>(key)) << '\n';
        else cache.set(static_cast<int>(key), static_cast<int>(value));
        ++i;
    }
}
