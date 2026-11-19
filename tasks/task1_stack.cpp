#include "../tasks.h"
#include "../common.h"
#include "../structures/stack.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

static string fixParentheses(const string& text) {
    StackDS stack;
    vector<bool> remove(text.size(), false);
    int position = 0;

    for (int i = 0; i < static_cast<int>(text.size()); ++i) {
        if (text[i] == '(') stack.push(i);
        else if (text[i] == ')' && !stack.pop(position)) remove[i] = true;
    }
    while (stack.pop(position)) remove[position] = true;

    string result;
    for (size_t i = 0; i < text.size(); ++i)
        if (!remove[i]) result += text[i];
    return result;
}

void runStackParentheses() {
    cout << "Введите строку: ";
    cout << "Результат: " << fixParentheses(readLine()) << '\n';
}
