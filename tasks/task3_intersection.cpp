#include "../tasks.h"
#include "../common.h"
#include "task2_set.h"
#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>

using namespace std;

void runSetIntersection() {
    cout << "Сколько множеств? ";
    int64_t countInput = readNum();
    if (countInput < 2 || countInput > 100) {
        cout << "Нужно от 2 до 100 множеств\n";
        return;
    }

    int count = static_cast<int>(countInput);
    auto sets = make_unique<IntSet[]>(count);
    for (int i = 0; i < count; ++i) {
        cout << "Множество " << i + 1 << " (числа через пробел): ";
        for (const string& text : words(readLine())) {
            int64_t value;
            if (parseInt64(text, value) && value > 0 && value <= numeric_limits<int>::max())
                sets[i].add(static_cast<int>(value));
        }
    }

    // Не сортируем элементы заново при сравнении каждой пары.
    vector<vector<int>> elements(count);
    for (int i = 0; i < count; ++i) elements[i] = sets[i].values();

    int first = 0, second = 1, best = -1;
    for (int i = 0; i < count; ++i) {
        for (int j = i + 1; j < count; ++j) {
            int common = 0;
            for (int value : elements[i])
                if (sets[j].has(value)) ++common;
            if (common > best) {
                best = common;
                first = i;
                second = j;
            }
        }
    }

    cout << "Множество " << first + 1 << ": "; sets[first].print();
    cout << "Множество " << second + 1 << ": "; sets[second].print();
    cout << "Общих элементов: " << best << '\n';
}
