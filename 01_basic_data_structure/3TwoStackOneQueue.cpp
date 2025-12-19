//
// Created by xyx on 2025/9/7.
//
#include <iostream>
#include <string>
#include <stack>
#include <vector>

using namespace std;

class Queue {
    stack<int> A;
    stack<int> B;

public:
    vector<string> enqueue() {
        A.push(0);
        return {"push A"};
    }

    vector<string> dequeue() {
        if (!B.empty()) {
            B.pop();
            return {"pop B"};
        }
        vector<string> op;
        while (A.size() > 1) {
            B.push(A.top());
            A.pop();
            op.emplace_back("move A B");
        }
        A.pop();
        op.emplace_back("pop A");
        return op;
    }
};

ostream &operator<<(ostream &out, const vector<string> &v) {
    for (int i = 0; i < v.size(); i++) {
        out << v[i];
        if (i < v.size() - 1) {
            out << endl;
        }
    }
    return out;
}

int main() {
    string op;
    Queue queue;
    while (cin >> op) {
        if (op == "enqueue") {
            cout << queue.enqueue() << endl;
        } else {
            cout << queue.dequeue() << endl;
        }
    }
    return 0;
}