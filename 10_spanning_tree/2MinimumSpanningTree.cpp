//
// Created by xyx on 2025/12/19.
//
#include <iostream>
#include <queue>
#include <vector>
#include <climits>

using namespace std;

int Prim(vector<vector<pair<int, int>>> &adj, int start) {
    vector dist(adj.size(), INT_MAX);
    dist[start] = 0;
    vector visited(adj.size(), false);

    auto cmp = [](const pair<int, int> &a, const pair<int, int> &b) {
        return a.second > b.second;
    };
    priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> queue(cmp);
    queue.emplace(start, 0);

    while (!queue.empty()) {
        auto [cur, dist_cur] = queue.top();
        queue.pop();
        if (visited[cur] || dist[cur] == INT_MAX) {
            continue;
        }

        visited[cur] = true;
        for (auto [linked, weight] : adj[cur]) {
            if (!visited[linked] && weight < dist[linked]) {
                dist[linked] = weight;
                queue.emplace(linked, weight);
            }
        }
    }

    int sum = 0;
    for (auto i : dist) {
        sum += i;
    }
    return sum;
}

int main() {
    int N, M;
    cin >> N >> M;
    vector<vector<pair<int, int>>> adj(N);

    int u, v, w;
    for (int i = 0; i < M; i++) {
        cin >> u >> v >> w;
        adj[u].emplace_back(v, w);
        adj[v].emplace_back(u, w);
    }
    cout << Prim(adj, 0) << '\n';
    return 0;
}