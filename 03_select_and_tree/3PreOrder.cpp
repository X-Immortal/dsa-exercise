//
// Created by xyx on 2025/10/14.
//
#include <iostream>

using namespace std;

int pre_order[100];
int p = 0;

void preOrder(int *in_order, int in_start, int in_end, int *post_order, int post_start, int post_end) {
    if (in_start > in_end) {
        return;
    }
    int root = pre_order[p++] = post_order[post_end];
    int root_index;
    for (root_index = in_start; root_index <= in_end && in_order[root_index] != root; root_index++);
    preOrder(in_order, in_start, root_index - 1, post_order, post_start, post_start + root_index - in_start - 1);
    preOrder(in_order, root_index + 1, in_end, post_order, post_start + root_index - in_start, post_end - 1);
}

int main() {
    int input[200];
    int N = 0;
    int count = 0;
    while (cin >> input[N++]) {
        count++;
        if (count >= 22) {
            break;
        }
    }
    preOrder(input, 0, N / 2 - 1, input + N / 2, 0, N / 2 - 1);
    for (int i = 0; i < N / 2; i++) {
        cout << pre_order[i];
        if (i != N / 2 - 1) {
            cout << " ";
        }
    }
    cout << endl;
    return 0;
}