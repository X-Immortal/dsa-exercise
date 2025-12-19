//
// Created by  Apple on 2025/9/23.
//
#include <iostream>

using namespace std;

int A[2048][2048], B[2048][2048], C[2048][2048];

void matrix_plus(int a[][2048], int xa, int ya, int b[][2048], int xb, int yb, int c[][2048], int xc, int yc, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            c[xc + i][yc + j] = a[xa + i][ya + j] + b[xb + i][yb + j];
        }
    }
}

void matrix_minus(int a[][2048], int xa, int ya, int b[][2048], int xb, int yb, int c[][2048], int xc, int yc, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            c[xc + i][yc + j] = a[xa + i][ya + j] - b[xb + i][yb + j];
        }
    }
}

void matrix_multiply(int a[][2048], int xa, int ya, int b[][2048], int xb, int yb, int c[][2048], int xc, int yc, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int sum = 0;
            for (int k = 0; k < n; ++k) {
                sum += a[xa + i][ya + k] * b[xb + k][yb + j];
            }
            c[xc + i][yc + j] = sum;
        }
    }
}

void strassen(int a[][2048], int xa, int ya, int b[][2048], int xb, int yb, int c[][2048], int xc, int yc, int n) {
    if (n <= 512) {
        matrix_multiply(a, xa, ya, b, xb, yb, c, xc, yc, n);
        return;
    }

    int sub_n = n / 2;
    int (*temp1)[2048] = new int[1024][2048];
    int (*temp2)[2048] = new int[1024][2048];
    // A12 - A22
    matrix_minus(a, xa, ya + sub_n, a, xa + sub_n, ya + sub_n, temp1, 0, 0, sub_n);
    // B21 + B22
    matrix_plus(b, xb + sub_n, yb, b, xb + sub_n, yb + sub_n, temp2, 0, 0, sub_n);
    // M7: C11
    strassen(temp1, 0, 0, temp2, 0, 0, c, xc, yc, sub_n);
    // A21 - A11
    matrix_minus(a, xa + sub_n, ya, a, xa, ya, temp1, 0, 0, sub_n);
    // B11 + B12
    matrix_plus(b, xb, yb, b, xb, yb + sub_n, temp2, 0, 0, sub_n);
    // M6: C22
    strassen(temp1, 0, 0, temp2, 0, 0, c, xc + sub_n, yc + sub_n, sub_n);
    // A11 + A22
    matrix_plus(a, xa, ya, a, xa + sub_n, ya + sub_n, temp1, 0, 0, sub_n);
    // B11 + B22
    matrix_plus(b, xb, yb, b, xb + sub_n, yb + sub_n, temp2, 0, 0, sub_n);
    // M1: C12
    strassen(temp1, 0, 0, temp2, 0, 0, c, xc, yc + sub_n, sub_n);
    // M1 + M7: C11
    matrix_plus(c, xc, yc + sub_n, c, xc, yc, c, xc, yc, sub_n);
    // M1 + M6: C22
    matrix_plus(c, xc, yc + sub_n, c, xc + sub_n, yc + sub_n, c, xc + sub_n, yc + sub_n, sub_n);
    // B21 - B11
    matrix_minus(b, xb + sub_n, yb, b, xb, yb, temp1, 0, 0, sub_n);
    // M4: temp2
    strassen(a, xa + sub_n, ya + sub_n, temp1, 0, 0, temp2, 0, 0, sub_n);
    // M1 + M7 + M4: C11
    matrix_plus(c, xc, yc, temp2, 0, 0, c, xc, yc, sub_n);
    // A11 + A12
    matrix_plus(a, xa, ya, a, xa, ya + sub_n, temp1, 0, 0, sub_n);
    // M5: C12
    strassen(temp1, 0, 0, b, xb + sub_n, yb + sub_n, c, xc, yc + sub_n, sub_n);
    // M1 + M7 + M4 - M5: C11
    matrix_minus(c, xc, yc, c, xc, yc + sub_n, c, xc, yc, sub_n);
    // A21 + A22
    matrix_plus(a, xa + sub_n, ya, a, xa + sub_n, ya + sub_n, temp1, 0, 0, sub_n);
    // M2: C21
    strassen(temp1, 0, 0, b, xb, yb, c, xc + sub_n, yc, sub_n);
    // M1 + M6 - M2: C22
    matrix_minus(c, xc + sub_n, yc + sub_n, c, xc + sub_n, yc, c, xc + sub_n, yc + sub_n, sub_n);
    // M2 + M4:C21, temp2free
    matrix_plus(c, xc + sub_n, yc, temp2, 0, 0, c, xc + sub_n, yc, sub_n);
    // B12 - B22
    matrix_minus(b, xb, yb + sub_n, b, xb + sub_n, yb + sub_n, temp1, 0, 0, sub_n);
    // M3: temp2
    strassen(a, xa, ya, temp1, 0, 0, temp2, 0, 0, sub_n);
    // M5 + M3:C12
    matrix_plus(c, xc, yc + sub_n, temp2, 0, 0, c, xc, yc + sub_n, sub_n);
    // M1 + M6 - M2 + M3:C22
    matrix_plus(c, xc + sub_n, yc + sub_n, temp2, 0, 0, c, xc + sub_n, yc + sub_n, sub_n);
    delete[] temp1;
    delete[] temp2;
}

int main() {
    int N;
    cin >> N;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> A[i][j];
        }
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> B[i][j];
        }
    }

    strassen(A, 0, 0, B, 0, 0, C, 0, 0, N);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << C[i][j];
            if (j < N - 1) {
                cout << " ";
            } else {
                cout << endl;
            }
        }
    }
    return 0;
}