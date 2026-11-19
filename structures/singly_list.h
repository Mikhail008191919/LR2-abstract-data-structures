#pragma once
struct SNode {
    int value;
    SNode* next;
    SNode(int v) : value(v), next(nullptr) {}
};
class SinglyList {
private:
    SNode* head;
    SNode* tail;
    int size;
public:
    SinglyList();
    ~SinglyList();
    SinglyList(const SinglyList&) = delete;
    SinglyList& operator=(const SinglyList&) = delete;
    void pushFront(int value);
    void pushBack(int value);
    bool insertBefore(int target, int value);
    bool insertAfter(int target, int value);
    bool popFront();
    bool popBack();
    bool eraseBefore(int target);
    bool eraseAfter(int target);
    bool removeValue(int value);
    bool contains(int value) const;
    bool get(int index, int& value) const;
    int length() const;
    void print() const;

    // Последовательный обход без поиска каждого элемента по индексу.
    template <typename Visitor>
    void forEach(Visitor visit) const {
        for (SNode* node = head; node != nullptr; node = node->next)
            visit(node->value);
    }
};
