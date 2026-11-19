#include "../tasks.h"
#include "../common.h"
#include "../structures/array.h"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

// Сортируем только индексы блоков; размеры остаются в IntArray из ЛР1.
static int64_t maxPyramid(const IntArray& widths, const IntArray& heights) {
    vector<int> order(widths.length());
    iota(order.begin(), order.end(), 0);

    sort(order.begin(), order.end(), [&](int a, int b) {
        int wa, wb, ha, hb;
        widths.get(a, wa);
        widths.get(b, wb);
        heights.get(a, ha);
        heights.get(b, hb);
        if (wa != wb) return wa > wb;
        return ha > hb;
    });

    int previousWidth = 0;
    int64_t totalHeight = 0;
    for (int index : order) {
        int width, height;
        widths.get(index, width);
        heights.get(index, height);
        if (width != previousWidth) {
            totalHeight += height;
            previousWidth = width;
        }
    }
    return totalHeight;
}

void runPyramid() {
    cout << "Количество блоков: ";
    int64_t count = readNum();
    if (count < 1 || count > 100000) {
        cout << "Неверное количество блоков\n";
        return;
    }

    IntArray widths;
    IntArray heights;
    cout << "Вводите ширину и высоту каждой строкой:\n";
    for (int64_t i = 0; i < count; ++i) {
        vector<string> input = words(readLine());
        int64_t width, height;
        if (input.size() != 2 ||
            !parseInt64(input[0], width) || !parseInt64(input[1], height) ||
            width <= 0 || height <= 0 || width > 1000000000 || height > 1000000000) {
            cout << "Ошибка ввода, повторите строку\n";
            if (!cin) return;
            --i;
            continue;
        }
        widths.pushBack(static_cast<int>(width));
        heights.pushBack(static_cast<int>(height));
    }
    cout << "Максимальная высота: " << maxPyramid(widths, heights) << '\n';
}
