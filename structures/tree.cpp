#include "tree.h"
#include <queue>

using namespace std;

BSTree::~BSTree() { clear(root); }

void BSTree::clear(Node* node) {
    if (node == nullptr) return;
    clear(node->left);
    clear(node->right);
    delete node;
}

void BSTree::insert(int value) {
    if (root == nullptr) {
        root = new Node(value);
        return;
    }

    Node* current = root;
    while (true) {
        if (value == current->value) return;
        if (value < current->value) {
            if (current->left == nullptr) {
                current->left = new Node(value);
                return;
            }
            current = current->left;
        } else {
            if (current->right == nullptr) {
                current->right = new Node(value);
                return;
            }
            current = current->right;
        }
    }
}

vector<int> BSTree::rightView() const {
    vector<int> result;
    if (root == nullptr) return result;

    queue<Node*> nodes;
    nodes.push(root);
    while (!nodes.empty()) {
        int levelSize = static_cast<int>(nodes.size());
        for (int i = 0; i < levelSize; ++i) {
            Node* current = nodes.front();
            nodes.pop();
            if (i == levelSize - 1) result.push_back(current->value);
            if (current->left != nullptr) nodes.push(current->left);
            if (current->right != nullptr) nodes.push(current->right);
        }
    }
    return result;
}
