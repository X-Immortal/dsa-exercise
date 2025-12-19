//
// Created by xyx on 2025/9/7.
//
#include <iostream>
#include <vector>
#include <stack>

using namespace std;

void move(stack<int> &a, stack<int> &b) {
    b.push(a.top());
    a.pop();
}

int main() {
    vector<int> result;
    int n;
    while (cin >> n) {
        result.push_back(n);
    }

    stack<int> in;
    stack<int> spur;
    for (int i = result.size(); i > 0; i--) {
        in.push(i);
    }

    for (int n : result) {
        while (!in.empty() && in.top() != n &&
            (spur.empty() || spur.top() != n)) {
            move(in, spur);
        }
        if (!in.empty() && n == in.top()) {
            in.pop();
        } else if (n == spur.top()) {
            spur.pop();
        } else {
            cout << "NO" << endl;
            return 0;
        }
    }
    cout << "YES" << endl;
    return 0;
}