//
// Created by xyx on 2025/10/28.
//
#include <cstdio>

using namespace std;

char arr[12500000] = "";

int main() {
    int num;
    int z = 0;
    while (scanf("%d", &num) != EOF) {
        arr[num >> 3] |= (1 << (num & 7));
        if (++z == 8) break;
    }
    bool flag = false;
    for (int i = 0; i < 12500000; i++) {
        for (int j = 0; j < 8; j++) {
            if (arr[i] & (1 << j)) {
                if (flag) {
                    putchar(' ');
                }
                printf("%d", (i << 3) | j);
                flag = true;
            }
        }
    }
    putchar('\n');
    return 0;
}
