//
// Created by xyx on 2025/11/30.
//
#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<int> a;
    int num;
    int sum = 0;
    while (cin >> num) {
        a.push_back(num);
        sum += num;
    }

    if (sum % 2 != 0) {
        cout << "0" << endl;
    }

    int target = sum / 2;
    vector<bool> dp(target + 1, false);
    dp[0] = true;

    for (int i : a) {
        for (int j = target; j >= i; j--) {
            if (dp[j - i]) {
                dp[j] = true;
            }
        }
    }

    cout << dp[target] << endl;

    return 0;
}