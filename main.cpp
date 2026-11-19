#include "common.h"
#include "tasks.h"
#include "tasks/task2_set.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    if (argc > 1) {
        if (!fileQuery(argc, argv)) {
            cout << "Формат: ./lr2 --file data.txt --query 'SETADD 10'\n";
        }
        return 0;
    }

    while (true) {
        cout << "\n1. Стек и скобки\n"
             << "2. Множество\n"
             << "3. Пересечение множеств\n"
             << "4. Пирамида\n"
             << "5. Правый край дерева\n"
             << "6. Хеш-таблицы\n"
             << "7. Метод свёртки\n"
             << "8. LFU-кэш\n"
             << "0. Выход\nВыбор: ";
        int number = readNum();
        if (!cin || number == 0) break;
        switch (number) {
            case 1: runStackParentheses(); break;
            case 2: runSetADT(); break;
            case 3: runSetIntersection(); break;
            case 4: runPyramid(); break;
            case 5: runTreeRightEdge(); break;
            case 6: runHashTable(); break;
            case 7: runHashFolding(); break;
            case 8: runLFUCache(); break;
            default: cout << "Нет такого задания\n";
        }
    }
}
