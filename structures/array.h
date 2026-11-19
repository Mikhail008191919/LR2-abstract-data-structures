#pragma once
class IntArray {
private:
    int* data;
    int size;
    int capacity;
    void grow();
public:
    IntArray();
    ~IntArray();
    IntArray(const IntArray&) = delete;
    IntArray& operator=(const IntArray&) = delete;
    void pushBack(int value);
    bool insert(int index, int value);
    bool get(int index, int& value) const;
    bool set(int index, int value);
    bool remove(int index);
    int length() const;
    void print() const;
};
