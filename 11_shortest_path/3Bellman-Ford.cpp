//
// Created by xyx on 2025/12/19.
//
#include <iostream>
#include <vector>
#include <climits>

using namespace std;

bool Bellman_Ford(vector<vector<pair<int, int>>> &adj, int start) {
    // 1. 为每个顶点设置一个初始距离，起点为0，其余为无穷大
    vector dist(adj.size(), INT_MAX);
    dist[start] = 0;

    // 3. 迭代次数为(顶点数-1)次
    for (int i = 0; i < adj.size(); ++i) {
        // 2. 遍历所有边，更新它们指向的顶点的距离
        for (int j = 0; j < adj.size(); ++j) {
            for (auto [linked, weight] : adj[j]) {
                int distance = dist[j] + weight;
                if (dist[j] != INT_MAX &&
                        distance < dist[linked]) {
                    // 额外增加一次迭代，若还能找到更小的距离，说明存在负环
                    if (i == adj.size() - 1) {
                        return true;
                    }
                    dist[linked] = distance;
                        }
            }
        }
    }

    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int F;
    cin >> F;

    int N, M, W;
    while (F--) {
        cin >> N >> M >> W;
        vector<vector<pair<int, int>>> adj(N);
        int S, E, T;
        for (int i = 0; i < M; i++) {
            cin >> S >> E >> T;
            adj[S - 1].emplace_back(E - 1, T);
            adj[E - 1].emplace_back(S - 1, T);
        }
        for (int i = 0; i < W; i++) {
            cin >> S >> E >> T;
            adj[S - 1].emplace_back(E - 1, -T);
        }

        cout << (Bellman_Ford(adj, 0) ? "YES" : "NO") << '\n';
    }

    return 0;
}