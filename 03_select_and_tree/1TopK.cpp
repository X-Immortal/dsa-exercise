//
// Created by xyx on 2025/10/14.
//
#include <iostream>
#include <queue>

using namespace std;

int main() {
    int K;
    cin >> K;

    priority_queue<int, vector<int>, greater<> > heap;
    int num;
    for (int i = 0; i < K; i++) {
        cin >> num;
        heap.push(num);
    }
    while (cin >> num) {
        if (num > heap.top()) {
            heap.pop();
            heap.push(num);
        }
    }

    cout << heap.top() << endl;

    return 0;
}