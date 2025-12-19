//
// Created by xyx on 2025/12/19.
//
#include <iostream>
#include <vector>
#include <climits>
#include <queue>

using namespace std;

vector<int> Dijkstra(vector<vector<pair<int, int>>> &adj, int start) {
    vector dist(adj.size(), INT_MAX);
    dist[start] = 0;

    // 优先级队列中存储<id, dist>
    auto cmp = [](const pair<int, int> &a, const pair<int, int> &b) {
        return a.second > b.second;
    };
    priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> queue(cmp);
    // 2. 将起点加入优先级队列
    queue.emplace(start, 0);

    while (!queue.empty()) {
        // 3. 从队列中获取距离最小的顶点
        auto [cur, dist_cur] = queue.top();
        queue.pop();
        // 排除已访问节点和不连通节点
        if (dist_cur > dist[cur] || dist[cur] == INT_MAX) {
            continue;
        }

        // 4. 将当前顶点从未访问集合中移除
        // 5. 更新当前顶点所有未访问邻居的距离
        for (auto [linked, weight] : adj[cur]) {
            int new_dist = dist_cur + weight;
            if (new_dist < dist[linked]) {
                dist[linked] = new_dist;
                // 将更新后的顶点入队（可能带来重复）
                queue.emplace(linked, new_dist);
            }
        }
    }

    return dist;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    int n, m, s, t;
    cin >> n >> m >> s >> t;
    vector<vector<pair<int, int>>> adj(n);

    int u, v, w;
    for (int i = 0; i < m; i++) {
        cin >> u >> v >> w;
        adj[u].emplace_back(v, w);
        adj[v].emplace_back(u, w);
    }

    auto &&result = Dijkstra(adj, s);
    cout << result[t] << '\n';

    return 0;
}