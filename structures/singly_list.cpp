#include "singly_list.h"
#include <iostream>
using namespace std;

// Односвязный список из первой лабораторной
SinglyList::SinglyList() : head(nullptr), tail(nullptr), size(0) {}
SinglyList::~SinglyList() {
    while (head != nullptr) {
        SNode* temp = head;
        head = head->next;
        delete temp;
    }
}
void SinglyList::pushFront(int value) {
    SNode* node = new SNode(value);
    node->next = head;
    head = node;
    if (tail == nullptr) tail = node;
    size++;
}
void SinglyList::pushBack(int value) {
    SNode* node = new SNode(value);
    if (tail == nullptr) head = tail = node;
    else {
        tail->next = node;
        tail = node;
    }
    size++;
}
bool SinglyList::insertBefore(int target, int value) {
    if (head == nullptr) return false;
    if (head->value == target) { pushFront(value); return true; }
    SNode* prev = head;
    SNode* cur = head->next;
    while (cur != nullptr && cur->value != target) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == nullptr) return false;
    SNode* node = new SNode(value);
    prev->next = node;
    node->next = cur;
    size++;
    return true;
}
bool SinglyList::insertAfter(int target, int value) {
    SNode* cur = head;
    while (cur != nullptr && cur->value != target) cur = cur->next;
    if (cur == nullptr) return false;
    SNode* node = new SNode(value);
    node->next = cur->next;
    cur->next = node;
    if (tail == cur) tail = node;
    size++;
    return true;
}
bool SinglyList::popFront() {
    if (head == nullptr) return false;
    SNode* temp = head;
    head = head->next;
    delete temp;
    size--;
    if (head == nullptr) tail = nullptr;
    return true;
}
bool SinglyList::popBack() {
    if (tail == nullptr) return false;
    if (head == tail) return popFront();
    SNode* cur = head;
    while (cur->next != tail) cur = cur->next;
    delete tail;
    tail = cur;
    tail->next = nullptr;
    size--;
    return true;
}
bool SinglyList::eraseBefore(int target) {
    if (head == nullptr || head->value == target) return false;
    SNode* prevPrev = nullptr;
    SNode* prev = head;
    SNode* cur = head->next;
    while (cur != nullptr && cur->value != target) {
        prevPrev = prev;
        prev = cur;
        cur = cur->next;
    }
    if (cur == nullptr) return false;
    if (prevPrev == nullptr) return popFront();
    prevPrev->next = cur;
    delete prev;
    size--;
    return true;
}
bool SinglyList::eraseAfter(int target) {
    SNode* cur = head;
    while (cur != nullptr && cur->value != target) cur = cur->next;
    if (cur == nullptr || cur->next == nullptr) return false;
    SNode* temp = cur->next;
    cur->next = temp->next;
    if (tail == temp) tail = cur;
    delete temp;
    size--;
    return true;
}
bool SinglyList::removeValue(int value) {
    if (head == nullptr) return false;
    if (head->value == value) return popFront();
    SNode* prev = head;
    SNode* cur = head->next;
    while (cur != nullptr && cur->value != value) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == nullptr) return false;
    prev->next = cur->next;
    if (tail == cur) tail = prev;
    delete cur;
    size--;
    return true;
}
bool SinglyList::contains(int value) const {
    SNode* cur = head;
    while (cur != nullptr) {
        if (cur->value == value) return true;
        cur = cur->next;
    }
    return false;
}
bool SinglyList::get(int index, int& value) const {
    if (index < 0 || index >= size) return false;
    SNode* cur = head;
    for (int i = 0; i < index; i++) cur = cur->next;
    value = cur->value;
    return true;
}
int SinglyList::length() const { return size; }
void SinglyList::print() const {
    cout << "[";
    SNode* cur = head;
    while (cur != nullptr) {
        cout << cur->value;
        if (cur->next != nullptr) cout << ", ";
        cur = cur->next;
    }
    cout << "]\n";
}
