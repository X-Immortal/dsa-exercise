//
// Created by xyx on 2025/11/30.
//
#include <iostream>

using namespace std;

int main() {
    int a;
    cin >> a;
    int c[105];
    int p = 0;
    while (cin >> c[p]) {
        p++;
    }

    int dp[1005] = {0};
    dp[0] = 1;
    for (int i = 0; i < p; i++) {
        for (int j = c[i]; j <= a; j++) {
            dp[j] += dp[j - c[i]];
        }
    }

    cout << dp[a] << endl;

    return 0;
}