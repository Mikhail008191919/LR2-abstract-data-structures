#include "../tasks.h"
#include "../common.h"
#include "../structures/tree.h"
#include <cstdint>
#include <iostream>
#include <limits>

using namespace std;

void runTreeRightEdge() {
    cout << "Введите числа дерева (0 — конец): ";
    BSTree tree;
    for (const string& text : words(readLine())) {
        int64_t value;
        if (!parseInt64(text, value)) continue;
        if (value == 0) break;
        if (value < numeric_limits<int>::min() || value > numeric_limits<int>::max())
            continue;
        tree.insert(static_cast<int>(value));
    }
    cout << "Правый край:";
    for (int value : tree.rightView()) cout << ' ' << value;
    cout << '\n';
}
