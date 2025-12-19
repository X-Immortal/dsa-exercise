//
// Created by xyx on 2025/12/19.
//
#include <iostream>
#include <vector>

using namespace std;

void Floyd(vector<vector<int>> &adj) {
    for (int mid = 0; mid < adj.size(); ++mid) {
        for (int begin = 0; begin < adj.size(); ++begin) {
            for (int end = 0; end < adj.size(); ++end) {
                int new_dist = adj[begin][mid] + adj[mid][end];
                if (new_dist < adj[begin][end]) {
                    if (begin == end) {
                        throw std::runtime_error("there is minus cycle in the graph");
                    }
                    adj[begin][end] = new_dist;
                }
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<int> A;
    A.reserve(M);
    int n;
    for (int i = 0; i < M; ++i) {
        cin >> n;
        A.push_back(n);
    }

    vector<vector<int>> D(N);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> n;
            D[i].push_back(n);
        }
    }

    Floyd(D);
    int ans = 0;
    for (int i = 1; i < M; ++i) {
        ans += D[A[i - 1] - 1][A[i] - 1];
    }
    cout << ans << '\n';

    return 0;
}