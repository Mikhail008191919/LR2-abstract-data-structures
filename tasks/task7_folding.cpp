#include "../tasks.h"
#include "../common.h"
#include "task6_hash.h"
#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

void runHashFolding() {
    cout << "Введите целый ключ: ";
    int64_t key = readNum();
    uint64_t rest = magnitude(key);
    vector<uint64_t> parts;
    do {
        parts.push_back(rest % 1000);
        rest /= 1000;
    } while (rest > 0);
    reverse(parts.begin(), parts.end());

    cout << "Части: ";
    for (size_t i = 0; i < parts.size(); ++i) {
        if (i) cout << " + ";
        cout << parts[i];
    }
    cout << " = " << fold(key) << '\n';
    cout << "Индекс корзины: " << fold(key) % 1009 << '\n';

    Cuckoo cuckoo(1009);
    cuckoo.add(key);
    cout << "Поиск в кукушке: " << (cuckoo.has(key) ? "TRUE" : "FALSE") << '\n';

    if (key >= numeric_limits<int>::min() && key <= numeric_limits<int>::max()) {
        ChainTable chain(1009);
        chain.add(static_cast<int>(key));
        cout << "Поиск в цепочках: "
             << (chain.has(static_cast<int>(key)) ? "TRUE" : "FALSE") << '\n';
    } else {
        cout << "Цепочки из ЛР1 хранят int, проверка пропущена\n";
    }
}
