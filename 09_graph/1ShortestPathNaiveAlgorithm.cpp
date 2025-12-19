//
// Created by xyx on 2025/12/15.
//
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <climits>

using namespace std;

vector<int> bfs(vector<vector<int>> &adj, int start) {
    vector dist(adj.size(), -1);
    vector visited(adj.size(), false);

    queue<int> q;
    q.push(start);
    int len = 0;
    while (!q.empty()) {
        int size = q.size();

        for (int i = 0; i < size; ++i) {
            int cur = q.front();
            q.pop();

            dist[cur] = len;

            for (int v : adj[cur]) {
                if (!visited[v]) {
                    q.push(v);
                    visited[v] = true;
                }
            }
        }

        len++;
    }

    return dist;
}


vector<int> Dijkstra(vector<vector<int>> &adj, int start) {
    // 1. 为每个顶点设置一个初始距离，起点为0，其余为无穷大
    vector dist(adj.size(), INT_MAX);
    dist[start] = 0;
    vector visited(adj.size(), false);

    // 优先级队列中存储<id, dist>
    // 此处应该存储距离的副本，而不应该让同一节点共享同一份距离，否则会破坏堆的特性
    auto cmp = [](const std::pair<int, int> &a, const std::pair<int, int> &b) {
        return a.second > b.second;
    };
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, decltype(cmp)> queue(cmp);
    // 2. 将起点加入优先级队列
    queue.emplace(start, 0);

    while (!queue.empty()) {
        // 3. 从队列中获取距离最小的顶点
        int cur = queue.top().first;
        queue.pop();

        // 排除已访问节点和不连通节点
        if (visited[cur] || dist[cur] == INT_MAX) {
            continue;
        }

        // 4. 将当前顶点从未访问集合中移除
        visited[cur] = true;
        // 5. 更新当前顶点所有未访问邻居的距离
        for (auto vertex : adj[cur]) {
            if (!visited[vertex]) {
                int distance = dist[cur] + 1;
                if (distance < dist[vertex]) {
                    dist[vertex] = distance;
                    // 将更新后的顶点入队（可能带来重复）
                    queue.emplace(vertex, distance);
                }
            }
        }
    }

    return dist;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<vector<int>> adj(N);

    int u, v;
    while (M--) {
        cin >> u >> v;
        if (--u == --v) {
            continue;
        }
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // 所有边的权重均为1，直接用bfs
    auto && result = Dijkstra(adj, 0);

    for (int i = 1; i < N; ++i) {
        cout << (result[i] == INT_MAX ? -1 : result[i]);
        if (i != N - 1) cout << " ";
    }
    cout << '\n';

    return 0;
}