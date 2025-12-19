//
// Created by xyx on 2025/12/16.
//
#include <iostream>
#include <vector>
#include <stack>

using namespace std;

enum State { UNVISITED, VISITING, VISITED };

bool dfs(vector<vector<int> > &adj, int start, vector<State> &visited, stack<int> &s) {
    if (visited[start] == VISITED) {
        return true;
    }
    if (visited[start] == VISITING) {
        return false;
    }

    visited[start] = VISITING;
    for (int v: adj[start]) {
        if (!dfs(adj, v, visited, s)) {
            return false;
        }
    }

    visited[start] = VISITED;
    s.push(start);
    return true;
}

bool dfs(vector<vector<int> > &adj, stack<int> &s) {
    vector visited(adj.size(), UNVISITED);
    for (int i = 0; i < adj.size(); ++i) {
        if (!dfs(adj, i, visited, s)) return false;
    }
    return true;
}

vector<int> topological_sort(vector<vector<int> > &adj) {
    stack<int> s;
    if (!dfs(adj, s)) {
        throw runtime_error("there is cycle");
    }

    vector<int> result;
    result.reserve(adj.size());
    while (!s.empty()) {
        result.push_back(s.top());
        s.pop();
    }
    return result;
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

    try {
        auto &&result = topological_sort(adj);
        cout << "NO" << '\n';
        for (int i = 0; i < result.size(); ++i) {
            cout << result[i] + 1;
            if (i != result.size() - 1) cout << " ";
        }
        cout << '\n';
    } catch (runtime_error &e) {
        cout << "YES" << '\n';
    }

    return 0;
}
