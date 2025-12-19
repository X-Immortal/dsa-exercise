//
// Created by xyx on 2025/9/7.
//
#include <iostream>
#include <string>

using namespace std;

class LinkedList {
    class Node {
        friend class LinkedList;
        friend ostream &operator<<(ostream &out, const LinkedList &list);

        int value;
        Node *next;

    public:
        Node(int value, Node *next) : value(value), next(next) {}
    };

    Node *head = new Node(0, nullptr);

public:
    ~LinkedList() {
        Node *cur = head;
        while (cur != nullptr) {
            Node *next = cur->next;
            delete cur;
            cur = next;
        }
    }

    void pushFront(int value) {
        Node *newNode = new Node(value, head->next);
        head->next = newNode;
    }

    void pushBack(int value) {
        Node *cur = head;
        while (cur->next != nullptr) {
            cur = cur->next;
        }
        Node *newNode = new Node(value, nullptr);
        cur->next = newNode;
    }

    bool search(int value) {
        return searchNode(value) != nullptr;
    }

    void remove(int value) {
        Node *prev = searchNode(value);
        if (prev != nullptr) {
            Node *cur = prev->next;
            prev->next = cur->next;
            delete cur;
        }
    }

    friend ostream &operator<<(ostream &out, const LinkedList &list);

private:
    Node *searchNode(int value) {
        for (Node *prev = head, *cur = head->next; cur != nullptr; prev = prev->next, cur = cur->next) {
            if (cur->value == value) {
                return prev;
            }
        }
        return nullptr;
    }
};

ostream &operator<<(ostream &out, const LinkedList &list) {
    for (LinkedList::Node *cur = list.head->next; cur != nullptr; cur = cur->next) {
        out << cur->value;
        if (cur->next != nullptr) {
            out << " ";
        }
    }
    return out << endl;
}

int main() {
    string op;
    int x;
    LinkedList list;
    while (cin >> op) {
        if (op == "pushFront") {
            cin >> x;
            list.pushFront(x);
        } else if (op == "pushBack") {
            cin >> x;
            list.pushBack(x);
        } else if (op == "search") {
            cin >> x;
            cout << list.search(x) << endl;
        } else if (op == "remove") {
            cin >> x;
            list.remove(x);
        } else {
            cout << list;
        }
    }
    return 0;
}