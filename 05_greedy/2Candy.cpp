//
// Created by xyx on 2025/11/18.
//
#include <iostream>
#include <algorithm>

using namespace std;

constexpr int sum(int begin, int end) {
    if (begin > end) return 0;
    return (end - begin + 1) * (end + begin) / 2;
}

int main() {
    int a[200];
    int n = 0;
    while (cin >> a[n]) {
        n++;
    }

    if (n < 2) {
        return n;
    }

    int candy = 0;
    int i = 0, j = 0;
    int last = 1;
    while (true) {
        // 找到最长递减序列
        for (j = i; j < n - 1 && a[j] > a[j + 1]; j++);
        if (i < j) {
            candy += sum(2, j - i);
            candy += max(j - i + 1, last);
            if (j == n - 1) {
                candy++;
                break;
            }
            i = j;
            last = 1;
            continue;
        }

        // 找到最长递增序列
        for (j = i; j < n - 1 && a[j] < a[j + 1]; j++);
        if (i < j) {
            candy += sum(1, j - i);
            if (j == n - 1) {
                candy += j - i + 1;
                break;
            }
            last = j - i + 1;
            i = j;
            continue;
        }

        // 找到最长平台
        for (j = i; j < n - 1 && a[j] == a[j + 1]; j++);
        if (i < j) {
            candy += j - i - 1 + last;
            if (j == n - 1) {
                candy++;
                break;
            }
            i = j;
            last = 1;
        }
    }

    cout << candy << endl;
    return 0;
}