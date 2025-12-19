//
// Created by xyx on 2025/12/19.
//
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

vector<int> coloring(vector<vector<int>> &adj) {
    // 用于记录每个节点的颜色
    vector color(adj.size(), -1);
    queue<int> q;

    // 随机选取一个节点染色为0，并加入队列
    color[0] = 0;
    q.push(0);

    while (!q.empty()) {
        // 取出当前节点
        int cur = q.front();
        q.pop();

        // 检测当前节点的相邻节点，若未染色则染为相反颜色，若已染色则检查是否冲突
        for (int v : adj[cur]) {
            if (color[v] == -1) {
                color[v] = 1 - color[cur];
                q.push(v);
            } else if (color[v] == color[cur]) {
                throw runtime_error("invalid graph");
            }
        }
    }

    return color;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<vector<int>> adj(N);

    int u, v;
    for (int i = 0; i < M; i++) {
        cin >> u >> v;
        adj[u - 1].push_back(v - 1);
        adj[v - 1].push_back(u - 1);
    }

    try {
        auto &&color = coloring(adj);
        cout << "YES" << '\n';

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < N; j++) {
                if (color[j] == i) {
                    cout << j + 1 << ' ';
                }
            }
            cout << '\n';
        }
    } catch (runtime_error &) {
        cout << "NO" << '\n';
    }

    return 0;
}