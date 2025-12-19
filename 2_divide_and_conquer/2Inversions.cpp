//
// Created by  Apple on 2025/9/22.
//
#include <iostream>
#include <cstring>

using namespace std;

int a[500005];
int buf[500005];
int pos = 0;

long long countInversions(long long l, long long r) {
    if (l >= r) {
        return 0;
    }
    long long count = 0;
    long long mid = (l + r) / 2;
    count += countInversions(l, mid);
    count += countInversions(mid + 1, r);

    long long i = l, j = mid + 1;
    pos = l;
    while (i <= mid && j <= r) {
        if (a[i] <= a[j]) {
            count += j - mid - 1;
            buf[pos++] = a[i];
            i++;
        } else {
            buf[pos++] = a[j];
            j++;
        }
    }
    count += (mid - i + 1) * (j - mid - 1);
    memcpy(buf + pos, a + i, (mid - i + 1) * sizeof(a[i]));
    memcpy(buf + pos, a + j, (r - j + 1) * sizeof(a[j]));
    memcpy(a + l, buf + l, (r - l + 1) * sizeof(a[l]));
    return count;
}

int main() {
    int N;
    cin >> N;
    for (int i = 0; i < N; i++) {
        cin >> a[i];
    }
    cout << countInversions(0, N - 1) << endl;
    return 0;
}