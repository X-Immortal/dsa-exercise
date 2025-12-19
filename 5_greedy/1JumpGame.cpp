//
// Created by xyx on 2025/11/18.
//
#include <iostream>

using namespace std;

int main() {
    int a[100];
    int n = 0;
    while (cin >> a[n]) {
        n++;
    }

    int jump_count = 0;
    for (int i = 0; i < n - 1; ) {
        if (i + a[i] >= n - 1) {
            jump_count++;
            break;
        }
        int next_jump = 1;
        for (int j = 2; j <= a[i]; j++) {
            next_jump = next_jump + a[i + next_jump] < j + a[i + j] ? j : next_jump;
        }
        i += next_jump;
        jump_count++;
    }

    cout << jump_count << endl;
    return 0;
}