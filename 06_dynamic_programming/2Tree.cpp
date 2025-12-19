//
// Created by xyx on 2025/11/30.
//
#include <iostream>
#include <vector>

using namespace std;

int n;
vector<vector<int>> graph;
vector<int> subtree;
vector<int> ans;

void dfs1(int current, int parent, int depth) {
    subtree[current] = 1;
    ans[0] += depth;
    for (int v : graph[current]) {
        if (v != parent) {
            dfs1(v, current, depth + 1);
            subtree[current] += subtree[v];
        }
    }
}

void dfs2(int current, int parent) {
    for (int v : graph[current]) {
        if (v != parent) {
            ans[v] = ans[current] + n - 2 * subtree[v];
            dfs2(v, current);
        }
    }
}


int main() {
    cin >> n;
    vector<vector<int>> edges(n - 1);
    int u, v;
    for (int i = 0; i < n - 1; i++) {
        cin >> u >> v;
        edges[i].push_back(u);
        edges[i].push_back(v);
    }

    graph.resize(n);
    for (auto &edge : edges) {
        graph[edge[0]].push_back(edge[1]);
        graph[edge[1]].push_back(edge[0]);
    }

    subtree.assign(n, 0);
    ans.assign(n, 0);
    dfs1(0, -1, 0);
    dfs2(0, -1);

    for (auto i : ans) {
        cout << i << " ";
    }
    cout << endl;

    return 0;
}