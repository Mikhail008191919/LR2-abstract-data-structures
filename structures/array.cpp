#include "array.h"
#include <iostream>
using namespace std;

// Динамический массив из первой лабораторной
IntArray::IntArray() : data(new int[4]), size(0), capacity(4) {}
IntArray::~IntArray() { delete[] data; }
void IntArray::grow() {
    capacity *= 2;
    int* temp = new int[capacity];
    for (int i = 0; i < size; i++) temp[i] = data[i];
    delete[] data;
    data = temp;
}
void IntArray::pushBack(int value) {
    if (size == capacity) grow();
    data[size++] = value;
}
bool IntArray::insert(int index, int value) {
    if (index < 0 || index > size) return false;
    if (size == capacity) grow();
    for (int i = size; i > index; i--) data[i] = data[i - 1];
    data[index] = value;
    size++;
    return true;
}
bool IntArray::get(int index, int& value) const {
    if (index < 0 || index >= size) return false;
    value = data[index];
    return true;
}
bool IntArray::set(int index, int value) {
    if (index < 0 || index >= size) return false;
    data[index] = value;
    return true;
}
bool IntArray::remove(int index) {
    if (index < 0 || index >= size) return false;
    for (int i = index; i < size - 1; i++) data[i] = data[i + 1];
    size--;
    return true;
}
int IntArray::length() const { return size; }
void IntArray::print() const {
    cout << "[";
    for (int i = 0; i < size; i++) {
        cout << data[i];
        if (i < size - 1) cout << ", ";
    }
    cout << "]\n";
}
