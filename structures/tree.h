#pragma once

#include <vector>


struct Node {
    int value;
    Node* left;
    Node* right;

    explicit Node(int value) : value(value), left(nullptr), right(nullptr) {}
};

class BSTree {
private:
    Node* root = nullptr;
    void clear(Node* node);

public:
    BSTree() = default;
    ~BSTree();
    BSTree(const BSTree&) = delete;
    BSTree& operator=(const BSTree&) = delete;

    void insert(int value);
    std::vector<int> rightView() const;
};
