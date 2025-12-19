//
// Created by xyx on 2025/12/16.
//
#include <iostream>
#include <vector>

using namespace std;

enum State { UNVISITED, VISITING, VISITED };

bool dfs(vector<vector<int> > &adj, int start, vector<State> &visited) {
    if (visited[start] == VISITED) {
        return false;
    }
    if (visited[start] == VISITING) {
        return true;
    }

    visited[start] = VISITING;
    for (int v: adj[start]) {
        if (dfs(adj, v, visited)) {
            return true;
        }
    }

    visited[start] = VISITED;
    return false;
}

bool has_cycle(vector<vector<int> > &adj) {
    vector visited(adj.size(), UNVISITED);
    for (int i = 0; i < adj.size(); ++i) {
        if (dfs(adj, i,visited)) return true;
    }
    return false;
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<vector<int> > adj(N);

    int u, v;
    while (M--) {
        cin >> u >> v;
        adj[u - 1].push_back(v - 1);
    }


    cout << (has_cycle(adj) ? "YES" : "NO") << '\n';

    return 0;
}
