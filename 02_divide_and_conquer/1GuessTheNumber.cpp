//
// Created by  Apple on 2025/9/22.
//
#include <iostream>
#include <climits>
#include <string>

using namespace std;

int main() {
    unsigned long long low = 0LL, high = ULONG_LONG_MAX;
    unsigned long long mid;
    string prompt;
    while (low <= high) {
        mid = low + (high - low) / 2;
        cout << mid << endl;
        cin >> prompt;
        if (prompt == "TooBig") {
            high = mid - 1;
        } else if (prompt == "TooSmall") {
            low = mid + 1;
        } else {
            break;
        }
    }
    return 0;
}