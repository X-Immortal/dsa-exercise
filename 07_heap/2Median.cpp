//
// Created by xyx on 2025/12/9.
//
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

class MedianFinder {
    priority_queue<int, vector<int>, less<>> left;
    priority_queue<int, vector<int>, greater<>> right;

public:
    MedianFinder() {
    }

    void addNum(int num) {
        if (left.empty() || num <= left.top()) {
            left.push(num);
        } else {
            right.push(num);
        }

        while (!right.empty() && right.size() > left.size()) {
            left.push(right.top());
            right.pop();
        }

        while (!left.empty() && left.size() - right.size() > 1) {
            right.push(left.top());
            left.pop();
        }
    }

    int findMedian() {
        return left.top();
    }
};

int main() {
    MedianFinder finder;

    int a;
    while (cin >> a) {
        finder.addNum(a);
        cout << finder.findMedian() << " " << endl;
    }
    return 0;
}