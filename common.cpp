#include "common.h"
#include <iostream>
#include <sstream>
using namespace std;

string upper(string text) {
    for (char& letter : text) {
        if (letter >= 'a' && letter <= 'z') {
            letter = char(letter - 'a' + 'A');
        }
    }
    return text;
}


vector<string> words(const string& text) {
    istringstream input(text);
    vector<string> result;
    string word;
    while (input >> word) result.push_back(word);
    return result;
}

bool parseInt64(const string& text, int64_t& number) {
    try {
        size_t used = 0;
        number = stoll(text, &used);
        return used == text.size();
    } catch (...) {
        return false;
    }
}

string readLine() {
    string text;
    getline(cin, text);
    return text;
}

int64_t readNum() {
    int64_t number = 0;
    string text = readLine();
    while (cin && !parseInt64(text, number)) {
        cout << "Ожидалось целое число, попробуйте снова: ";
        text = readLine();
    }
    return number;
}
