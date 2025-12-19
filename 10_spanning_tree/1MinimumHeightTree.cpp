//
// Created by xyx on 2025/12/19.
//
#include <iostream>
#include <vector>

using namespace std;

int depth = 0;
int point;
int max_depth = 0;
void dfs(vector<vector<int>> &adj, int start, vector<bool> &visited) {
    visited[start] = true;
    if (depth > max_depth) {
        max_depth = depth;
        point = start;
    }
    for (int v : adj[start]) {
        if (!visited[v]) {
            depth++;
            dfs(adj, v, visited);
            depth--;
        }
    }
}

int dfs(vector<vector<int>> &adj) {
    vector visited(adj.size(), false);
    // 从任意节点开始做一次dfs，找到最大深度的节点，它一定是树的直径端点
    dfs(adj, 0, visited);
    depth = 0;
    visited.assign(adj.size(), false);
    // 从直径端点开始再做一次dfs，递归的最大深度就是树的直径长度
    dfs(adj, point, visited);
    // 树的最小高度就是树的直径长度的一半
    return (max_depth + 1) / 2;
}

int main() {
    int N;
    cin >> N;
    vector<vector<int>> adj(N);

    int u, v;
    while (cin >> u >> v) {
        adj[u].emplace_back(v);
        adj[v].emplace_back(u);
    }

    cout << dfs(adj) << '\n';

    return 0;
}