#pragma once

struct StackNode {
    int value;
    StackNode* next;
    StackNode(int v) : value(v), next(nullptr) {}
};

class StackDS {
private:
    StackNode* top;
public:
    StackDS();
    ~StackDS();
    StackDS(const StackDS&) = delete;
    StackDS& operator=(const StackDS&) = delete;
    void push(int value);
    bool pop(int& value);
    bool empty() const;
    void print() const;
};
