#include "stack.h"
#include <iostream>
using namespace std;

// Стек из первой лабораторной
StackDS::StackDS() : top(nullptr) {}
StackDS::~StackDS() {
    while (top != nullptr) {
        StackNode* temp = top;
        top = top->next;
        delete temp;
    }
}
void StackDS::push(int value) {
    StackNode* node = new StackNode(value);
    node->next = top;
    top = node;
}
bool StackDS::pop(int& value) {
    if (top == nullptr) return false;
    StackNode* temp = top;
    value = temp->value;
    top = top->next;
    delete temp;
    return true;
}
bool StackDS::empty() const { return top == nullptr; }
void StackDS::print() const {
    cout << "[";
    StackNode* cur = top;
    while (cur != nullptr) {
        cout << cur->value;
        if (cur->next != nullptr) cout << ", ";
        cur = cur->next;
    }
    cout << "]\n";
}
